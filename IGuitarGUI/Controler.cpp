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
    m_track = 0;


	m_timer = new QTimer(this);
	m_muted = false;
	m_playing = false;

	m_configuration = new Configuration();

	m_scene = new PlayerScene(this);
	m_view = new MyView(m_scene);

	m_songManager = new SongManager(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(ticTac()));
	connect(m_timer, SIGNAL(timeout()), m_songManager, SLOT(checkTime()));
	connect(m_timer, SIGNAL(timeout()), m_songManager, SLOT(compareChordWithPlayed()));

    connect(m_songManager, SIGNAL(updateChord(TrackChord*)), this, SLOT(currentChordSlot(TrackChord*)));
    connect(m_songManager, SIGNAL(updateStats(int, int)), this, SLOT(updateStats(int,int)));
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
    chord->setPlaying();


    m_scene->setSceneToChord(chord);
}

/**
 * @brief Controler::updateStats
 * @param nombre valides, nombre joues
 *
 * Met a jour l'interface avec le nombre d'accords joues
 *
 */
void Controler::updateStats(int validated, int played) {
    m_scene->updateStats(validated, played);
}

/**
 * @brief Controler::ticTac
 *
 * Slot appelé à intervalle régulier pour permettre la mise à jour de l'interface.
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
	m_timer->stop();
	m_songManager->pause();
	m_playing=false;
}

/**
 * @brief Controler::stopSong
 *
 * Arrete le morceau
 */
void Controler::stopSong()
{
	m_timer->stop();
	m_songManager->stop();
    m_playing=false;
}

/**
 * @brief Controler::elapsedTime
 * @return Le temps écoulé depuis la lecture du morceau
 *
 * Donne le temps écoulé depuis le début de la lecture du morceau.
 */
int Controler::elapsedTime()
{
	//return m_clockOffset + m_globalClock.elapsed();
	return m_songManager->getElapsedTime();
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
	//m_globalClock.start();
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
