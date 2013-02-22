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

    m_mustPlay = true;
    m_mustStop = false;
    m_playMuted = false;

    m_scoreManager = NULL;

    m_scene = new PlayerScene(this);
    m_view = new myView(m_scene);

    m_view->show(); // Que se passe-t-il apres le show ?
    timeOut();      // timeOut est il execute ?


}

/**
  * @brief MainWidget::playScore
  *
  * @param mute
  *
  */

void Controler::playScore(bool mute)
{

	m_playMuted = mute;
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


// cette fonction m'a l'air vraiment sale...
void Controler::timeOut()
{
	if (m_mustPlay)
	{
		m_mustPlay = false;

		std::map<std::string, std::string> multiTracksMap;
		std::vector<std::string> muteTracks;

		if (m_playMuted) {
			muteTracks.push_back("all");
		}

        QString path;
        path = "Tracks/BeatlesDayInTheLife/Beatles.xml";

		multiTracksMap["all"] =  "Tracks/BeatlesDayInTheLife/AllTracks.wav";

		MusicManager* musicManager = new MusicManager(multiTracksMap, muteTracks);
		m_scoreManager = new ScoreManager(musicManager);


         m_scoreManager->loadScore("Tracks/BeatlesDayInTheLife/Guitar.txt");

//		LogicalTrack *tr = new LogicalTrack();
//		TrackLoader::convertXmlToLogicalTrack("Tracks/BeatlesDayInTheLife/test.xml", tr);
//		m_scoreManager->loadScore(tr);

        LogicalTrack *tr = new LogicalTrack();
		qDebug() << path;
		TrackLoader::convertXmlToLogicalTrack(path, tr);

		m_scoreManager->loadScore(tr);
		m_scoreManager->run();

		// nécessaire pour pas que ça crash, pourquoi ? (jm)
		usleep(100000);

		m_scoreManager->setNextPart("[VERSE1]");
		m_renderAreas.changeButtonMode(false);
	}

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

		m_renderAreas.drawScore(currentScore,
								m_scoreManager->getCurrentChord(),
								m_scoreManager->getCurrentPart(),
								m_scoreManager->getNextPart(),
								mustGoToTheNextPart);
	}

	if (m_mustStop)
	{
		m_mustStop = false;

		if (m_scoreManager != NULL)
		{

			m_scoreManager->stop();
			m_scoreManager = NULL;

			m_renderAreas.changeButtonMode(true);
		}
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

QList<QString> Controler::getChordList() //TODO
{
    QList<QString> chordList;
    for(int i=0; i<10; i++) {
        chordList.append("A");
    }
    //return m_scoreManager->chordList();
    return chordList;
}

QString Controler::getChords() //TODO
{
    //return m_scoreManager->chords();
    return "G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H";
}


// amoi

int Controler::elapsedTime() {
    return clockOffset + globalClock.elapsed();
}

void Controler::startClock() {
    globalClock.start();
}

void Controler::pauseClock() {
    clockOffset += globalClock.elapsed();
}