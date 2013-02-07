#include "MainWidget.hpp"
#include "MainWindow.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>

#include <string>


#include <unistd.h>

/**
  *@brief MainWidget::~MainWidget
  *
  *Destructeur par défaut
  */
MainWidget::~MainWidget()
{

}

/**
  * @brief MainWidget::MainWidget
  *
  * Constructeur
  */
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->parent = parent;
	m_mustPlay = false;
	m_mustStop = false;
	m_playMuted = false;

	m_scoreManager = NULL;

	buildGUI();
}

/**
  * @brief MainWidget::playScore
  *
  * @param mute
  *
  */

void MainWidget::playScore(bool mute)
{

	m_playMuted = mute;
	m_mustPlay = true;

}
/**
  * @brief MainWidget::stopScore
  *
  *
  */
void MainWidget::stopScore()
{
	m_mustStop = true;
}

/**
  * @brief MainWidget::buildGuI
  *
  * Construit le widget principal
  */

void
MainWidget::buildGUI()
{
	createActions();

	QGridLayout *topLayout = new QGridLayout;
	topLayout->addWidget(&m_renderAreas, 0, 0);

	m_renderAreas.loadChordImages(CHORDS_IMAGES_LOCATION);
	setLayout(topLayout);
}



void
MainWidget::createActions()
{
	unsigned int timeOutTime = 0; //pourquoi? m-à-j toutes les 50 ms ?
	// marche très bien sur mon pc avec maj continue, et sera beaucoup plus agréable à l'oeil

	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()) );
	m_timer->start( timeOutTime );
}


// cette fonction m'a l'air vraiment sale...
void MainWidget::timeOut()
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
        if(((MainWindow *) parent) != 0){
           path = ((MainWindow *) parent)->m_conf.getSongName();
        }
        else{
            path = "Tracks/BeatlesDayInTheLife/Beatles.xml";
        }
        multiTracksMap["all"] = path.toStdString().c_str();

		MusicManager* musicManager = new MusicManager(multiTracksMap, muteTracks);
		m_scoreManager = new ScoreManager(musicManager);


	//	 m_scoreManager->loadScore("Tracks/BeatlesDayInTheLife/Guitar.txt");

//		LogicalTrack *tr = new LogicalTrack();
//		TrackLoader::convertXmlToLogicalTrack("Tracks/BeatlesDayInTheLife/test.xml", tr);
//		m_scoreManager->loadScore(tr);

        LogicalTrack *tr = new LogicalTrack();
		TrackLoader::convertXmlToLogicalTrack("Tracks/BeatlesDayInTheLife/BeatlesOK.xml", tr);

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


void MainWidget::timeOutSlot()
{
	timeOut();
}

void
MainWidget::initListeners()
{

}
