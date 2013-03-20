#include "SongManager.h"
#include "Configuration.h"
#include "Controler.hpp"
#include <map>
#include <vector>
#include <string>
#include <unistd.h>

SongManager::SongManager(QObject* parent): QObject(parent),
	m_track(0),
	m_musicManager(0),
	m_currentPart(0),
	m_currentChord(0),
	m_currentInputChord(0),
	m_chordControl(new chord_ctrl()),

	number_of_valid_chord_checks(0),
	number_of_chord_checks(0),

	precision_in_ms(100),
	elapsedTime(0)

{
}

SongManager::~SongManager()
{
	delete m_chordControl;
	delete m_musicManager;
}


void SongManager::load(LogicalTrack* track)
{
	elapsedTime = 0;
	number_of_valid_chord_checks = 0;
	m_track = track;

	std::map<std::string, std::string> multiTracksMap;
	std::vector<std::string> muteTracks;

	multiTracksMap["all"] =  m_track->getAudioFileName().toStdString();

	if(m_musicManager != 0 )
	{
		delete m_musicManager;
		sleep(2); //sécurité pour portaudio
	}

	m_musicManager = new MusicManager(multiTracksMap, muteTracks, -1, -1);

	m_musicManager->run();
	goToChord(m_track->getPartTrackList()[0]->getTrackChordsList()[0]);
}

// on démarre le timer, la lecture, et on réactive les threads
void SongManager::play()
{
	m_musicManager->start();
	m_musicManager->play();
}

// on stoppe timer, lecture, threads et on revient à l'accord précédent
// (permettre une configuration du comportement) : partie / accord / rien
void SongManager::pause()
{
	m_musicManager->pause();

	switch(((Controler*)parent())->getConfiguration()->getPauseSetting())
	{
		case PAUSE_TO_SAME_TIME:
			break;
		case PAUSE_TO_LAST_CHORD:
			goToChord(m_currentChord);
			break;
		case PAUSE_TO_LAST_PART:
			goToChord(m_currentPart->getTrackChordsList()[0]);
			break;
		default:
			break;
	}
}

// on stoppe timer, lecture, threads et on revient au début
void SongManager::stop()
{
	elapsedTime = 0;
	m_musicManager->pause();

	goToChord(m_track->getPartTrackList()[0]->getTrackChordsList()[0]);
}

// on coupe le son
void SongManager::mute(bool b)
{
	m_musicManager->mute(b);
}

// on set les accords et parties à ce qui correspond
void SongManager::goToChord(TrackChord* chord)
{
	int msPosition = 0;
	// on doit trouver la partie de l'accord
	QList<PartTrack*>::iterator iPart;
	QList<TrackChord*>::iterator iChord;

	for(iPart = m_track->getPartTrackList().begin();
		iPart != m_track->getPartTrackList().end();
		++iPart)
	{
		QList<TrackChord*> gtc = (*iPart)->getTrackChordsList();
		for(iChord = gtc.begin();
			iChord != gtc.end();
			++iChord)
		{
			msPosition = (*iChord)->getBeginningInMs();
			if(((TrackChord*) *iChord) == chord)
			{
				m_currentPart = *iPart;
				m_currentChord = *iChord;

				m_musicManager->goToInMs(msPosition);
				emit updateChord(m_currentChord);

				elapsedTime = msPosition;
				return;
			}
		}
	}

	// normalement on n'est pas sensé arriver ici
}

// compare la note jouée avec la note actuelle. incrémente le pourcentage de réussite si réussi. (à voir en fonction du nombre d'appels dans l'accord)
void SongManager::compareChordWithPlayed()
{
	++number_of_chord_checks;
	double buffer[INPUT_FRAMES_PER_BUFFER];
	chord_init(m_chordControl, SAMPLE_RATE, INPUT_FRAMES_PER_BUFFER, INPUT_FRAMES_PER_BUFFER);

	m_musicManager->fillBufferWithLastInputValues(buffer, INPUT_FRAMES_PER_BUFFER);

	chroma_compute(m_chordControl, buffer, INPUT_FRAMES_PER_BUFFER);

	if(m_currentInputChord != 0) delete m_currentInputChord;
	m_currentInputChord = new BasicChord(chord_compute(m_chordControl));
	emit currentlyPlayedChord(*m_currentInputChord);

	if( m_currentInputChord->toString() == m_currentChord->getChord() )
	{
		++number_of_valid_chord_checks;
	}
}

// vérifie dans quel accord on se situe et met à jour.
void SongManager::checkTime()
{
	elapsedTime += m_time.restart();

	int msPrevPosition = 0;
	int msPosition = 0;

	QList<PartTrack*>::iterator iPart;
	QList<TrackChord*>::iterator iChord;

	for(iPart = m_track->getPartTrackList().begin();
		iPart != m_track->getPartTrackList().end();
		++iPart)
	{
		QList<TrackChord*> gtc = (*iPart)->getTrackChordsList();
		for(iChord = gtc.begin();
			iChord != gtc.end();
			++iChord)
		{
			if((iChord + 1) != gtc.end())
			{
				msPosition = (*(iChord + 1))->getBeginningInMs(); // vérifier si on n'a pas un décalage avec le début.
			}
			else if ((iPart + 1)  != m_track->getPartTrackList().end())
			{
				msPosition = (*(*(iPart + 1))->getTrackChordsList().begin())->getBeginningInMs();
			}
			else
			{
				return;
			}

			if(msPrevPosition <= elapsedTime && elapsedTime < msPosition)
			{
				if(m_currentChord != *iChord)
				{
					emit lastChordCorrectness((double) number_of_valid_chord_checks / (double)number_of_chord_checks);
					emit updateChord(*iChord);

					number_of_chord_checks = 0;
					number_of_valid_chord_checks = 0;
				}
				m_currentPart = *iPart;
				m_currentChord = *iChord;

				return;
			}
			msPrevPosition = msPosition;
		}
	}
}
