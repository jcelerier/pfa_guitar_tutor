#include "Controler.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>
#include <string>
#include <unistd.h>

/**
  *@brief MainWidget::~MainWidget
  *
  *Destructeur par défaut
  */
Controler::~Controler()
{
}

/**
  * @brief MainWidget::MainWidget
  *
  * Constructeur
  */
Controler::Controler()
{
    clockOffset = 0;

    PlayerChord* tempChord;

    for(int i=0; i<10; i++) {
        tempChord = new PlayerChord();
        tempChord->name = "A";
        tempChord->time = i * 1000;
        chordList.append(*tempChord);
    }



    m_mustPlay = false;
    m_mustStop = false;
    m_playing = false;

    initSong();

     chordList = getChordList(track);

    m_scene = new PlayerScene(this);
    m_view = new myView(m_scene);

    QTimer *t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(ticTac()));
    t_Timer->start(1000/Configuration::framesPerSec);

    m_view->show(); // Que se passe-t-il apres le show ?

}

void Controler::ticTac() {
    if(m_playing)
        m_scene->advance();
    //m_scene->setPlayedChord("QS");
    m_scene->setPlayedChord(QString(m_scoreManager->getCurrentChord().c_str()));
}

/**
  * @brief MainWidget::playScore
  *
  * @param mute
  *
  */

void Controler::playScore(bool mute)
{

	m_mustPlay = true;

}
/**
  * @brief MainWidget::stopScore
  *
  *
  */
void Controler::stopScore()
{
	m_mustStop = true;
}

void Controler::initSong() {

    track = new LogicalTrack();
    m_scoreManager = NULL;

    std::map<std::string, std::string> multiTracksMap;
    std::vector<std::string> muteTracks;

    /*if (m_playMuted) {
        muteTracks.push_back("all");
    }*/

    QString path;
    path = "Tracks/BeatlesDayInTheLife/Beatles.xml";

    multiTracksMap["all"] =  "Tracks/BeatlesDayInTheLife/AllTracks.wav";


    MusicManager* musicManager = new MusicManager(multiTracksMap, muteTracks);
    m_scoreManager = new ScoreManager(musicManager);


    m_scoreManager->loadScore("Tracks/BeatlesDayInTheLife/Guitar.txt");

//		LogicalTrack *tr = new LogicalTrack();
//		TrackLoader::convertXmlToLogicalTrack("Tracks/BeatlesDayInTheLife/test.xml", tr);
//		m_scoreManager->loadScore(tr);


    qDebug() << path;
    TrackLoader::convertXmlToLogicalTrack(path, track);

    m_scoreManager->loadScore(track);

    qDebug() << "ammiiii";
    qDebug() << QString(m_scoreManager->ScoreToString(track).c_str());
    qDebug() << getChordList(track)[0].name;
}

void Controler::startSong() {
    m_scoreManager->run();

    // nécessaire pour pas que ça crash, pourquoi ? (jm)
    usleep(100000);

    m_scoreManager->setNextPart("[VERSE1]");
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
