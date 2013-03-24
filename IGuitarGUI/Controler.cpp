#include "Controler.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>
#include <QString>
#include <Track/TrackLoader.h>
#include <PlayerScene.h>
#include <QMessageBox>

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
	m_clockOffset = 0;
	m_track = 0;
	played_chords_in_current_part = 0;
	well_played_chords_in_current_part = 0;

	m_totalPlayedChords = 0;
	m_totalValidatedChords = 0;

	is_at_beginning = true;
	m_timer = new QTimer(this);
	m_paused = false;
	m_muted = false;
	m_playing = false;

	m_configuration = new Configuration();

	m_scene = new PlayerScene(this);
	m_view = new MyView(m_scene);

	m_songManager = new SongManager(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(ticTac()));
	connect(m_timer, SIGNAL(timeout()), m_songManager, SLOT(checkTime()));
	connect(m_timer, SIGNAL(timeout()), m_songManager, SLOT(compareChordWithPlayed()));

	connect(m_songManager, SIGNAL(nonNaturalChange(TrackChord*)), this, SLOT(setChordPosition(TrackChord*)));
	connect(m_songManager, SIGNAL(updateChord(TrackChord*)), this, SLOT(currentChordSlot(TrackChord*)));
	connect(m_songManager, SIGNAL(lastChordCorrectness(TrackChord*, double)), this, SLOT(victoryPercent(TrackChord*, double)));
	connect(m_songManager, SIGNAL(currentlyPlayedChord(BasicChord)), m_scene, SLOT(setPlayedChord(BasicChord)));

	m_view->show();
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
	if(chord == chord->part()->getTrackChordsList()[0]
	   && chord->previous() != 0
	   && m_configuration->getLoopSetting()
	   && well_played_chords_in_current_part < chord->part()->getTrackChordsList().count() )
	{
		m_songManager->goToChord(chord->part()->previous()->getTrackChordsList()[0]);
		well_played_chords_in_current_part = 0;
		m_totalPlayedChords--;
	}
	else if(chord->previous() != 0
			&& chord->previous()->part() != chord->part()) //on passe dans une nouvelle partie
	{
		well_played_chords_in_current_part = 0;
	}


	setChordPosition(chord);
	chord->setPlaying();

	//emit repaintSong();
	m_scene->setSceneToChord(chord);
}

void Controler::setChordPosition(TrackChord* chord)
{
	int validatedToDelete = 0;
	TrackChord* iChord = chord;
	do
	{
		if(iChord->isValidated())
			validatedToDelete ++;

		iChord->setPlayed(false);
		iChord->setPlaying(false);
		iChord->validate(false);

	} while((iChord = iChord->next()) != 0);

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
	}

	m_scene->updateStats(m_totalValidatedChords, m_totalPlayedChords);
	m_totalPlayedChords++;
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

	QString path = QFileDialog::getOpenFileName(0, tr("Loading"), m_configuration->getSongDirectory(), tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);

	if(path.isNull())
	{
		return false;
	}

	stopSong();

	if(m_track != 0) delete m_track;
	m_track = new LogicalTrack();

	if(!(TrackLoader::convertXmlToLogicalTrack(path, m_track))) {
		QMessageBox::information(0, tr("Error"), tr("An error occured while loading the XML file. You should open it again with the editor and check its content."));
		m_track = 0;
		return false;
	}
	if(!QFileInfo(m_track->getAudioFileName()).exists()) {
		QMessageBox::information(0, tr("Error"), tr("The audio file does not exist. Please open the xml file with the editor to change the path to the audio file."));
		delete m_track;
		m_track = 0;
		return false;
	}

	/* Boucle de base pour la lecture des Track;
	TrackChord* iChord = m_track->getPartTrackList()[0]->getTrackChordsList()[0];
	do
	{

	} while((iChord = iChord->next()) != 0);
	*/
	m_songManager->load(m_track);
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


	m_globalClock.start();
	m_songManager->play();
	m_timer->start(1000/Configuration::framesPerSec);
	m_playing=true;
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
	m_totalPlayedChords = 0;
	m_totalValidatedChords = 0;
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


TrackChord* Controler::getCurrentChord()
{
	return m_songManager->getCurrentChord();
}
