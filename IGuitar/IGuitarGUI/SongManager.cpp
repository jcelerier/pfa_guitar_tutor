#include "SongManager.h"
#include <QDebug>
#include <map>
#include <vector>
#include <string>
#include <unistd.h>

SongManager::SongManager():
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
	m_timer.setInterval(precision_in_ms);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkTime()));
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(compareChordWithPlayed()));
}

SongManager::~SongManager()
{
	delete m_chordControl;
	delete m_musicManager;

	qDebug() << "songmanager properly deleted";
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
	m_time.start();
	m_timer.start();

	m_musicManager->start();
	m_musicManager->play();
}

// on stoppe timer, lecture, threads et on revient à l'accord précédent
// (permettre une configuration du comportement) : partie / accord / rien
void SongManager::pause()
{
	m_timer.stop();
	m_musicManager->pause();

	//if(PAUSE_COMPORTMENT == 0)
	{
		//ne rien faire si on veut repartir du même endroit
		//goToChord(m_currentPart->getTrackChordsList()[0]); // pour repartir du début de la partie
		goToChord(m_currentChord); // pour repartir du début de l'accord en cours.
	}
}

// on stoppe timer, lecture, threads et on revient au début
void SongManager::stop()
{
	m_timer.stop();
	elapsedTime = 0;
	m_musicManager->pause();

	goToChord(m_track->getPartTrackList()[0]->getTrackChordsList()[0]);
}

// on coupe le son
void SongManager::mute(bool)
{

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

	qDebug() << "ALERT: goToChord went to end. UNDEFINED COMPORTMENT";

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
