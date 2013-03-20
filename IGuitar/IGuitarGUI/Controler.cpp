#include "Controler.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>
#include <string>
#include <unistd.h>

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

	qDebug() << "controler properly deleted";
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
	m_scoreManager = 0;
	m_musicManager = 0;
	m_track = 0;
	m_currentPart.clear();
	m_timer = new QTimer(this);
	m_paused = false;

	m_songManager = new SongManager();
	connect(m_timer, SIGNAL(timeout()), this, SLOT(ticTac()));
	m_configuration = new Configuration();

	connect(m_songManager, SIGNAL(updateChord(TrackChord*)), this, SLOT(currentChordSlot(TrackChord*)));
	connect(m_songManager, SIGNAL(lastChordCorrectness(double)), this, SLOT(victoryPercent(double)));

	restartEngine();
}

void Controler::currentChordSlot(TrackChord* c)
{
	qDebug() << c->getChord();
}

void Controler::victoryPercent(double d)
{
	qDebug() << "réussite : " << d *100 << "%";
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


	   /* QStringList playedChord = m_scoreManager->getCurrentChord();
		m_scene->setPlayedChord(playedChord);*/
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

	m_songManager->load(m_track);


	//load ici

	return true;
}

/**
 * @brief Controler::startSong
 *
 * Démarre la lecture de la partition et de la musique.
 */

void Controler::startSong()
{
	qDebug() << "Controler::startSong()";
	m_songManager->play();


	m_timer->start(1000/Configuration::framesPerSec);
}

/**
 * @brief Controler::stopSong
 *
 * Suspend le player.
 */
void Controler::pauseSong()
{
	qDebug() << "Controler::pauseSong()";
	m_songManager->pause();

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
 //       m_scoreManager->run();
		m_timer->start();
		startSong();
		m_playing=true;
	}
	else
	{
		pauseClock();
		pauseSong();
		m_playing=false;

	}
	m_globalClock.start();
}

/**
 * @brief Controler::pauseClock
 *
 * Pause l'horloge. -- JM : de même
 */
void Controler::pauseClock()
{
	m_timer->stop();

	m_clockOffset += m_globalClock.elapsed();
	m_savedClock = m_clockOffset;
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

			   if(chord != "n") {
				   chList.append(*tempChord);
			   }
			   delete tempChord;

		   }
	   }
   }
   return chList;
}

// l'idéal serait de ne pas faire reset la view et juste de mettre à jour les infos
// mais comme LogicalTrack est utilisé directement on ne peut pas pour l'instant
// il faut aussi faire gaffe à la désactivation de portaudio dans MusicManager
void Controler::restartEngine()
{
	m_timer->stop();
	m_clockOffset = 0;

	m_playing = false;

	if(!initSong())
	{
		exit(0);
		// note : ne pas appeler les méthodes de qApp (quit, exit...) car qApp->exec() n'est pas encore appelé
	}

	m_chordList = getChordList(m_track);

	if (m_scene != 0) delete m_scene;
	if (m_view != 0) delete m_view;

	m_scene = new PlayerScene(this);
	m_view = new MyView(m_scene);

	m_timer->start(1000/Configuration::framesPerSec);

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
