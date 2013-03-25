#include "SongManager.h"
#include "Configuration.h"
#include "Controler.hpp"

#include <QMap>
#include <QVector>
#include <QString>


/**
 * @brief SongManager::SongManager
 * @param parent
 *
 * Constructeur du gestionnaire de partition
 */
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
	m_elapsedTime(0)

{
}

SongManager::~SongManager()
{
	delete m_chordControl;
	delete m_musicManager;
}


/**
 * @brief SongManager::load
 * @param track LogicalTrack à charger.
 *
 * Charge un morceau. Attention, simple recopie de pointeur : ne pas faire de delete.
 */
void SongManager::load(LogicalTrack* track)
{
	m_elapsedTime = 0;
	number_of_valid_chord_checks = 0;
	m_track = track;

	QMap<QString, QString> multiTracksMap;
	QVector<QString> muteTracks;

	multiTracksMap["all"] =  m_track->getAudioFileName();

	if(m_musicManager != 0 )
	{
		delete m_musicManager;
	}

	m_musicManager = new MusicManager(multiTracksMap, muteTracks, -1, -1);

	m_musicManager->run();
}

/**
 * @brief SongManager::play
 *
 * Démarre la lecture. Normalement le timer est activé depuis le controleur.
 */
void SongManager::play()
{
	m_musicManager->start();
	m_musicManager->play();
	m_time.restart();
	m_isFirstChord = true;
}

/**
 * @brief SongManager::pause
 *
 * Pause la lecture et mets le player dans l'état défini dans la configuration :
 * -> On reprend au même instant
 * -> On reprend au début de l'accord qu'on jouait
 * -> On reprend au début de la partie
 */
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

/**
 * @brief SongManager::stop
 *
 * Arrête la lecture et remet au début.
 */
void SongManager::stop()
{
	m_musicManager->pause();

	if(m_track != 0)
	{
		goToChord(m_track->getPartTrackList()[0]->getTrackChordsList()[0]);
	}
}

/**
 * @brief SongManager::mute
 * @param b Booléen : true si on coupe le son.
 *
 * Gère le mutage - démutage du son.
 */
void SongManager::mute(bool b)
{
	m_musicManager->mute(b);
}

/**
 * @brief SongManager::goToChord
 * @param chord Accord ou on veut se déplacer
 *
 * Déplace la lecture à l'accord passé en paramètre
 */
void SongManager::goToChord(TrackChord* chord)
{
	int msPosition = chord->getBeginningInMs();
	m_musicManager->goToInMs(msPosition);
	m_elapsedTime = msPosition;
	m_time.restart();

	// on doit trouver la partie de l'accord

	m_currentPart = chord->part();
	m_currentChord = chord;

	emit nonNaturalChange(m_currentChord);
	emit updateChord(m_currentChord);
}

void SongManager::goToBeginning()
{
	m_musicManager->goToInMs(0);
	m_elapsedTime = 0;
	m_time.restart();
	emit nonNaturalChange(m_currentChord);
	emit updateChord(m_currentChord);
}



/**
 * @brief SongManager::compareChordWithPlayed
 *
 * Compare la note jouée avec la note actuelle.
 * Incrémente le pourcentage de réussite si réussi.
 * (à voir en fonction du nombre d'appels dans l'accord).
 *
 * Emet un signal à chaque fois.
 */
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


/**
 * @brief SongManager::checkTime
 *
 * Vérifie à chaque tick dans quel accord on se trouve.
 */
void SongManager::checkTime()
{
	m_elapsedTime += m_time.restart();

	int chordStartInMs = 0;
	int chordEndInMs = 0;

	TrackChord* iChord = m_track->getPartTrackList()[0]->getTrackChordsList()[0];
	do
	{
		chordStartInMs = iChord->getBeginningInMs();

		if(iChord->next() != 0)
		{
			chordEndInMs = iChord->next()->getBeginningInMs();
		}
		else
		{
			chordEndInMs = m_track->getEnd();
		}

		// Si le temps écoulé est dans l'accord listé
		if(chordStartInMs <= m_elapsedTime && m_elapsedTime < chordEndInMs)
		{
			// Si cet accord est différend de l'accord actuel
			if(m_currentChord != iChord || (m_currentChord == m_track->getPartTrackList()[0]->getTrackChordsList()[0] && m_isFirstChord))
			{
				m_isFirstChord = false;
				// On émet la réussite de l'accord précédent
				emit lastChordCorrectness(m_currentChord, (double) number_of_valid_chord_checks / (double)number_of_chord_checks);

				// On émet le nouvel accord
				emit updateChord(iChord);

				number_of_chord_checks = 0;
				number_of_valid_chord_checks = 0;

				m_currentPart =iChord->part();
				m_currentChord = iChord;
			}
			return;
		}
	} while((iChord = iChord->next()) != 0);

}

/**
 * @brief SongManager::getCurrentChord
 *
 * Gère le mutage - démutage du son.
 */
TrackChord* SongManager::getCurrentChord()
{
	return m_currentChord;
}

int SongManager::getElapsedTime() {
	return m_elapsedTime;
}
