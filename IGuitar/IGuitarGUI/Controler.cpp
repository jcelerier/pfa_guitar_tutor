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
}

/**
  * @brief Controler::Controler
  *
  * Constructeur
  */
Controler::Controler()
{

	clockOffset = 0;

	m_mustPlay = false;
	m_mustStop = false;
	m_playing = false;

	if(!initSong())
	{
		exit(0);
		// note : ne pas appeler les méthodes de qApp (quit, exit...) car qApp->exec() n'est pas encore appelé
	}


	chordList = getChordList(track);

	m_scene = new PlayerScene(this);
	m_view = new myView(m_scene);

	QTimer *t_Timer = new QTimer(this);
	connect(t_Timer, SIGNAL(timeout()), this, SLOT(ticTac()));
	t_Timer->start(1000/Configuration::framesPerSec);
	m_view->show();
	// Que se passe-t-il apres le show ?
	// JM: l'event loop de Qt est lancée
}

void Controler::ticTac() {
	m_scoreManager->update();
	if(m_playing)
		m_scene->updateScene();
	//m_scene->setPlayedChord("QS");
	QString playedChord = QString(m_scoreManager->getCurrentChord().c_str());
	m_scene->setPlayedChord(playedChord);
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

	track = new LogicalTrack();
	m_scoreManager = NULL;

	std::map<std::string, std::string> multiTracksMap;
	std::vector<std::string> muteTracks;

	/*if (m_playMuted) {
		muteTracks.push_back("all");
	}*/


	//m_scoreManager = new ScoreManager(musicManager);


//    m_scoreManager->loadScore("Tracks/BeatlesDayInTheLife/Guitar.txt");

//		LogicalTrack *tr = new LogicalTrack();
//		TrackLoader::convertXmlToLogicalTrack("Tracks/BeatlesDayInTheLife/test.xml", tr);
//		m_scoreManager->loadScore(tr);

	TrackLoader::convertXmlToLogicalTrack(path, track);
	multiTracksMap["all"] =  track->getAudioFileName().toStdString();
	MusicManager* musicManager = new MusicManager(multiTracksMap, muteTracks);
	m_scoreManager = new ScoreManager(musicManager);
	m_scoreManager->loadScore(track);

	return true;
}

void Controler::startSong() {
	m_scoreManager->run();

	// nécessaire pour pas que ça crash, pourquoi ? (jm)
	usleep(100000);

	m_scoreManager->setNextPart(track->getPartName(2).toStdString());
	//m_renderAreas.changeButtonMode(false);
}

void Controler::stopSong() {
	m_mustStop = false;

	if (m_scoreManager != NULL)
	{

		m_scoreManager->stop();
		m_scoreManager = NULL;

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
		initSong();
	}
}

void Controler::pauseClock() {
	clockOffset += globalClock.elapsed();
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

			   tempChord->name = chord;
			   tempChord->time = (int) time;

			   if(chord != "n") {
				   chList.append(*tempChord);
			   }

		   }
	   }
   }
   return chList;
}
