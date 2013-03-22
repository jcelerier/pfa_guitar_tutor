#include "Controler.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>
#include <QString>
#include <Track/TrackLoader.h>

/**
 * @brief Controler::~Controler
 *
 * Destructeur par défaut
 */
Controler::~Controler()
{
	delete m_timer;
	delete m_configuration;

	delete m_songManager;
	if(m_scene != 0) delete m_scene;
	if(m_view  != 0) delete m_view;
	if(m_track != 0) delete m_track;
}

/**
 * @brief Controler::Controler
 *
 * Constructeur
 */
Controler::Controler()
{
	m_view = 0;
	m_scene = 0;
	m_track = 0;
	played_chords_in_current_part = 0;
	well_played_chords_in_current_part = 0;

	m_totalPlayedChords = 0;
	m_totalValidatedChords = 0;

	is_at_beginning = 0;
	m_timer = new QTimer(this);
	m_paused = false;
	m_muted = false;

	m_songManager = new SongManager(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(ticTac()));
	connect(m_timer, SIGNAL(timeout()), m_songManager, SLOT(checkTime()));
	connect(m_timer, SIGNAL(timeout()), m_songManager, SLOT(compareChordWithPlayed()));

	connect(m_songManager, SIGNAL(nonNaturalChange(TrackChord*)), this, SLOT(resetValidatedNotes(TrackChord*)));
	connect(m_songManager, SIGNAL(updateChord(TrackChord*)), this, SLOT(currentChordSlot(TrackChord*)));
	connect(m_songManager, SIGNAL(lastChordCorrectness(TrackChord*, double)), this, SLOT(victoryPercent(TrackChord*, double)));

	m_configuration = new Configuration();

	restartEngine();

	connect(m_songManager, SIGNAL(nonNaturalChange(TrackChord*)), m_scene, SLOT(goToChord(TrackChord*)));

}

/**
 * @brief Controler::getConfiguration
 * @return La classe de configuration
 */
Configuration* Controler::getConfiguration()
{
	return m_configuration;
}

/**
 * @brief Controler::currentChordSlot
 * @param chord Le nouvel accord
 *
 * Ce slot reçoit à chaque fois l'accord dans lequel on entre.
 * Il effectue la vérification pour savoir si on passe à la partie suivante.
 */
void Controler::currentChordSlot(TrackChord* chord)
{
	TrackChord* iChord = chord;
	do
	{
		iChord->setPlayed(false);
		iChord->setPlaying(false);
		iChord->validate(false);

	} while((iChord = iChord->next()) != 0);

	chord->setPlaying();
	QList<PartTrack*>::iterator iPart;

	for(iPart = m_track->getPartTrackList().begin();
		iPart != m_track->getPartTrackList().end();
		++iPart)

	{
		//Dans l'ordre :  si on est à un début de partie, autre que la première, qu'on loop, et qu'on a mal joué
		if(chord == (*iPart)->getTrackChordsList()[0]
				&& chord != m_track->getPartTrackList()[0]->getTrackChordsList()[0]
				&& m_configuration->getLoopSetting()
				&& well_played_chords_in_current_part < (*iPart)->getTrackChordsList().count() )
		{
			QList<TrackChord*> cList = (*(iPart - 1))->getTrackChordsList();
			iChord = *(cList.begin());
			m_songManager->goToChord(iChord);

			do
			{
				iChord->setPlayed(false);
				iChord->setPlaying(false);
				iChord->validate(false);

			} while((iChord = iChord->next()) != 0);

			well_played_chords_in_current_part = 0;
		}
	}

	m_totalPlayedChords++;
	m_scene->updateStats(m_totalValidatedChords, m_totalPlayedChords);
}

void Controler::resetValidatedNotes(TrackChord* chord)
{
	QList<PlayerChord>::iterator iChord;

	bool toReset = false;
	int validatedToDelete = 0;

	for(iChord = m_chordList.begin();
		iChord != m_chordList.end();
		++iChord)

	{
		if(chord == iChord->getTrackChord())
			toReset=true;
		if(toReset) {
			if(iChord->isValidated())
				validatedToDelete ++;

			iChord->validate(false);

		}
	}
	m_totalPlayedChords -= validatedToDelete;
}


/**
 * @brief Controler::victoryPercent
 * @param d Pourcentage de réussite de l'accord précédent.
 *
 * Si ce pourcentage est supérieur à celui défini dans la configuration, on ajoute une réussite.
 */
void Controler::victoryPercent(TrackChord* chord, double d)
{
	if(chord != 0)
	{
		chord->setPlaying(false);
		if(d * 100 > m_configuration->getDifficulty())
		{
			chord->validate();
			m_totalValidatedChords++;

			++well_played_chords_in_current_part;
		}
	chord->setPlayed();
	emit repaintSong();
	}
}

/**
 * @brief Controler::ticTac
 *
 * Slot appelé à intervalle régulier pour permettre la mise à jour de l'interface et du scoreManager
 */
void Controler::ticTac()
{
	if(m_playing)
	{
		m_scene->updateScene();
	}
}

/**
 * @brief Controler::initSong
 * @return Vrai si et seulement si une track a été choisie par l'utilisateur.
 *
 * Demande la track à ouvrir et la charge dans le player.
 */
bool Controler::initSong()
{
	QString path = QFileDialog::getOpenFileName(0, tr("Loading"), ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);

	if(path.isNull())
	{
		return false;
	}

	if(m_track != 0) delete m_track;
	m_track = new LogicalTrack();

	TrackLoader::convertXmlToLogicalTrack(path, m_track);

	/* Boucle de base pour la lecture des Track;
	TrackChord* iChord = m_track->getPartTrackList()[0]->getTrackChordsList()[0];
	do
	{

	} while((iChord = iChord->next()) != 0);
	*/
	m_songManager->load(m_track);
	m_chordList = getChordList(m_track);

	m_scene->loadSong(m_track);

	return true;
}

/**
 * @brief Controler::startSong
 *
 * Démarre la lecture de la partition et de la musique.
 */

void Controler::startSong()
{
	if(is_at_beginning)
	{
		m_songManager->goToChord(m_track->getPartTrackList()[0]->getTrackChordsList()[0]);
		is_at_beginning = false;
	}

	m_songManager->play();
	m_timer->start(1000/Configuration::framesPerSec);
	m_playing=true;
	m_globalClock.start();
}

/**
 * @brief Controler::pauseSong
 *
 * Suspend le player.
 */
void Controler::pauseSong()
{
	m_clockOffset += m_globalClock.elapsed();
	m_timer->stop();
	m_songManager->pause();
	m_playing=false;
}

/**
 * @brief Controler::stopSong
 *
 * Arr$ete le morceau
 */
void Controler::stopSong()
{
	m_clockOffset += m_globalClock.elapsed();
	m_timer->stop();
	m_clockOffset = 0;
	m_songManager->stop();
	m_playing=false;
	is_at_beginning = true;
}

/**
 * @brief Controler::elapsedTime
 * @return Le temps écoulé depuis la lecture du morceau
 *
 * Donne le temps écoulé depuis le début de la lecture du morceau.
 */
int Controler::elapsedTime()
{
	return m_clockOffset + m_globalClock.elapsed();
}

/**
 * @brief Controler::switchPlaying
 *
 * Alterne entre lecture et pause.
 */
void Controler::switchPlaying()
{
	if(!m_playing)
	{
		startSong();
	}
	else
	{
		pauseSong();
	}
	m_globalClock.start();
}

/**
 * @brief Controler::pauseClock
 *
 * Pause l'horloge.
 */
void Controler::pauseClock()
{

}

/**
 * @brief Controler::getChordList
 * @return La liste des accords
 *
 * Accesseur pour la liste des accords du morceau.
 */
QList<PlayerChord> *Controler::getChordList()
{
	return &m_chordList;
}

/**
 * @brief Controler::getChordList
 * @param trackName Track
 * @return La liste des accords de la track.
 *
 * Converti une LogicalTrack en une liste de PlayerChord.
 */
QList<PlayerChord> Controler::getChordList(LogicalTrack* trackName)
{
	QList<PlayerChord> chList;
	PlayerChord* tempChord;

	QList<PartTrack*>::iterator it1;
	QList<TrackChord*>::iterator it2;

	QList<PartTrack*> partTrackList = trackName->getPartTrackList();

	for(it1 = partTrackList.begin(); it1 != partTrackList.end(); ++it1)
	{

		QList<TrackChord*> gtc = (*it1)->getTrackChordsList(); //utilisée dans la boucle qui suit, plante si pas de passage par variable intermédiaire (pourquoi?) --- hamid

		for(it2 = gtc.begin(); it2 != gtc.end(); ++it2)
		{
			for(int i = 0; i < (*it2)->getRepetition(); i++)
			{
				tempChord = new PlayerChord();
				qreal time;
				QString chord("");

				chord += (*it2)->getChord();
				time = (*it2)->getBeginningInMs();

				tempChord->setName(chord);
				tempChord->setTime((int) time);
				tempChord->setTrackChord(*it2);

				if(chord != "n") {
					chList.append(*tempChord);
				}
				delete tempChord;

			}
		}
	}
	return chList;
}

/**
 * @brief Controler::mute
 *
 * Coupe le son
 */
void Controler::mute()
{
	m_songManager->mute(true);
	m_muted = true;
}

/**
 * @brief Controler::unmute
 *
 * Remet le son
 */
void Controler::unmute()
{
	m_songManager->mute(false);
	m_muted = false;
}

/**
 * @brief Controler::muteState
 * @return Etat du son (coupé ou non)
 */
bool Controler::muteState()
{
	return m_muted;
}

// l'idéal serait de ne pas faire reset la view et juste de mettre à jour les infos
// mais comme LogicalTrack est utilisé directement on ne peut pas pour l'instant
// il faut aussi faire gaffe à la désactivation de portaudio dans MusicManager
void Controler::restartEngine()
{
	m_timer->stop();
	m_clockOffset = 0;

	m_playing = false;

	if (m_scene != 0) delete m_scene;
	if (m_view != 0) delete m_view;

	m_scene = new PlayerScene(this);
	m_view = new MyView(m_scene);

/*	if(!initSong())
	{
		exit(0);
		// note : ne pas appeler les méthodes de qApp (quit, exit...) car qApp->exec() n'est pas encore appelé
	}
*/

	connect(m_songManager, SIGNAL(currentlyPlayedChord(BasicChord)), m_scene, SLOT(setPlayedChord(BasicChord)));

	m_view->show();
}

/**
 * @brief Controler::getTrack
 * @return La logicalTrack utilisée
 *
 * Accesseur pour la logicalTrack courante.
 */
LogicalTrack* Controler::getTrack()
{
	return m_track;
}

TrackChord* Controler::getFirstChord()
{
	return m_track->getFirstChord();
}
