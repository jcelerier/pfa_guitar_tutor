#include "Controler.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>
#include <string>
#include <unistd.h>

/**
  *@brief Controler::~Controler
  *
  *Destructeur par défaut
  */
Controler::~Controler()
{
    if(m_musicManager != 0) delete m_musicManager;
    if(m_scoreManager != 0) delete m_scoreManager;
    delete m_timer;
    delete m_configuration;
    delete m_audioConfiguration;
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
	m_scoreManager = 0;
	m_musicManager = 0;
    m_track = 0;
    m_timer = new QTimer(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(ticTac()));
    m_configuration = new Configuration();
    m_audioConfiguration = new AudioConfiguration(m_configuration, (QWidget*) this);

	restartEngine();
}

void Controler::ticTac() {
    if(m_scoreManager->isRunning()) {
		m_scene->updateScene();
        m_scoreManager->update();
        QString playedChord = QString(m_scoreManager->getCurrentChord().c_str());
        m_scene->setPlayedChord(playedChord);
    }
}

/**
  * @brief Controler::playScore
  *
  * @param mute
  *
  */

void Controler::playScore(bool mute)
{
	m_mustPlay = true;
}
/**
  * @brief Controler::stopScore
  *
  *
  */
void Controler::stopScore()
{
	m_mustStop = true;
}

bool Controler::initSong() {

	QString path = QFileDialog::getOpenFileName(0, tr("Loading"), ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);


	if(path.isNull())
	{
		return false;
	}

    if(m_track != 0) delete m_track;
    m_track = new LogicalTrack();

	std::map<std::string, std::string> multiTracksMap;
	std::vector<std::string> muteTracks;

    TrackLoader::convertXmlToLogicalTrack(path, m_track);

    multiTracksMap["all"] =  m_track->getAudioFileName().toStdString();

	if(m_musicManager != 0)
	{
		m_musicManager->stop();
		/*qDebug() << "musicManager deletion";
		delete m_musicManager;
		qDebug() << "musicManager deleted";*/
	}
	else
	{
        m_musicManager = new MusicManager(multiTracksMap, muteTracks, m_configuration->getInputIndex(), m_configuration->getOutputIndex());
	}
	if(m_scoreManager != 0) delete m_scoreManager;
	m_scoreManager = new ScoreManager(m_musicManager);
    m_scoreManager->loadScore(m_track);

	return true;
}


void Controler::openAudioOptions()
{
    m_audioConfiguration->show();
}

void Controler::startSong()
{
	m_scoreManager->run();


	// nécessaire pour pas que ça crash, pourquoi ? (jm)
	usleep(100000);

    m_scoreManager->setNextPart(m_track->getPartName(2).toStdString());
    m_timer->start(1000/Configuration::framesPerSec);
	//m_renderAreas.changeButtonMode(false);
}

void Controler::stopSong()
{
	m_mustStop = false;
    m_timer->stop();

	if (m_scoreManager != 0)
	{

		m_scoreManager->stop();
/*
		delete m_scoreManager;
		m_scoreManager = 0;
*/
		//m_renderAreas.changeButtonMode(true);
	}
}

// cette fonction m'a l'air vraiment sale...
void Controler::timeOut()
{
	if (m_scoreManager != NULL)
	{
		prioritizedScore currentScore;

		m_scoreManager->getScore(currentScore, MIN_SCORE_BOUND, MAX_SCORE_BOUND);
		m_scoreManager->update();

		bool mustGoToTheNextPart = (m_scoreManager->getValidatedNotesPercent() >= PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART);
		if(mustGoToTheNextPart)
		{
			m_scoreManager->setToNaturalNextPart();
		}

		/*m_renderAreas.drawScore(currentScore,
								m_scoreManager->getCurrentChord(),
								m_scoreManager->getCurrentPart(),
								m_scoreManager->getNextPart(),
								mustGoToTheNextPart);*/
	}

}


void Controler::timeOutSlot()
{
	timeOut();
}

void
Controler::initListeners()
{

}

// amoi

int Controler::elapsedTime() {
	return clockOffset + globalClock.elapsed();
}

void Controler::startClock() {
	if(!m_playing) {
		startSong();
		globalClock.start();
		m_playing=true;
	}
	else {
		stopSong();
		m_playing=false;
//		restartEngine(); // à remplacer par une fonction qui fait juste redémarrer le morceau
	}
}

void Controler::pauseClock() {
    clockOffset += globalClock.elapsed();
}

QList<PlayerChord> *Controler::getChordList()
{
    return &chordList;
}

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
			   time = (*it2)->getDuration();

               tempChord->setName(chord);
               tempChord->setTime((int) time);

			   if(chord != "n") {
				   chList.append(*tempChord);
			   }

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
	qDebug( ) << "here";
    m_timer->stop();
	clockOffset = 0;

	m_mustPlay = false;
	m_mustStop = false;
	m_playing = false;

	if(!initSong())
	{
		exit(0);
		// note : ne pas appeler les méthodes de qApp (quit, exit...) car qApp->exec() n'est pas encore appelé
	}
	qDebug( ) << "there";


    chordList = getChordList(m_track);

	if (m_scene != 0) delete m_scene;
	if (m_view != 0) delete m_view;

	m_scene = new PlayerScene(this);
	m_view = new myView(m_scene);

    m_timer->start(1000/Configuration::framesPerSec);

	qDebug( ) << "ok fine";
	m_view->show();
}

LogicalTrack* Controler::getTrack()
{
    return m_track;
}
