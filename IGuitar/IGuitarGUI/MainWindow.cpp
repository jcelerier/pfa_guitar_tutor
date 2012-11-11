#include "MainWindow.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>

MainWindow::~MainWindow()
{

}

MainWindow::MainWindow()
: QWidget()
{  
	m_mustPlay = false;
	m_mustStop = false;
	m_playMuted = false;

	m_scoreManager = NULL;

	buildGUI();


}

void MainWindow::playScore(bool mute)
{
	if (m_scoreManager == NULL) {
		m_playMuted = mute;
		m_mustPlay = true;


	}
}

void MainWindow::stopScore()
{
	if (m_scoreManager != NULL) {
		m_mustStop = true;


	}
}

void
MainWindow::buildGUI()
{
	setWindowTitle(tr("IGuitar"));

	setMinimumSize(1024, 768);

	// Set background colour to black
	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::black);
	setPalette(p);

	createActions();
	createMenus();
	createToolBars();
	createDockWindows();

	QGridLayout *topLayout = new QGridLayout;
	topLayout->addWidget(&m_renderAreas, 0, 0);

	m_renderAreas.loadChordImages(CHORDS_IMAGES_LOCATION);
	setLayout(topLayout);
}



void
MainWindow::createActions()
{
	unsigned int timeOutTime = 50;

	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()) );
	m_timer->start( timeOutTime );
}

void MainWindow::timeOut()
{
	//m_scoreManager->update();
	//std::cout << m_scoreManager->getCurrentChord() << std::endl;
	//	m_openGLWidget->repaint();

	//	  m_renderAreas[0]->setFillGradient(Qt::red, Qt::green);

	if (m_mustPlay) {
		m_mustPlay = false;

		std::map<std::string, std::string> multiTracksMap;
		std::vector<std::string> muteTracks;

		if (m_playMuted) {
			muteTracks.push_back("all");
		}
		////	multiTracksMap["guitar"] = "Tracks/MuseHysteria/Guitar.wav";
		////	multiTracksMap["bass"] = "Tracks/MuseHysteria/Bass.wav";
		////	multiTracksMap["background"] = "Tracks/MuseHysteria/Background.wav";
		multiTracksMap["all"] = "Tracks/BeatlesDayInTheLife/AllTracks.wav";

		MusicManager* musicManager = new MusicManager(multiTracksMap, muteTracks);
		m_scoreManager = new ScoreManager(musicManager);

                m_scoreManager->loadScore("Tracks/BeatlesDayInTheLife/Guitar.txt");
                // lancement de la chanson...
                //m_scoreManager->loadScore(ui->comboBox->currentText());
		m_scoreManager->run();

		usleep(100000);

		m_scoreManager->setNextPart("[VERSE1]");

		m_renderAreas.changeButtonMode(false);
	}

	if (m_scoreManager != NULL) {
		prioritizedScore currentScore;
		m_scoreManager->getScore(currentScore, MIN_SCORE_BOUND, MAX_SCORE_BOUND);

		m_scoreManager->update();

		bool mustGoToTheNextPart = (m_scoreManager->getValidatedNotesPercent() >= PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART);
		if(mustGoToTheNextPart) {
			m_scoreManager->setToNaturalNextPart();
		}

		m_renderAreas.drawScore(currentScore, m_scoreManager->getCurrentChord(), m_scoreManager->getCurrentPart(), m_scoreManager->getNextPart(), mustGoToTheNextPart);
	}

	if (m_mustStop) {
		m_mustStop = false;

		if (m_scoreManager != NULL) {

			m_scoreManager->stop();

			//delete m_playBackScoreManager;
			//delete m_musicManager;

			m_scoreManager = NULL;

			m_renderAreas.changeButtonMode(true);
		}
	}




//	std::cout << m_scoreManager->getValidatedNotesPercent() << std::endl;


	//	std::cout << "*************" << std::endl;
	//	while (!currentScore.empty()) {
	//		PrioritizedNote currentNote = currentScore.top();
	//
	//		std::cout << "(" << currentNote.isInCurrentPart
	//				  << ") " << currentNote.chord
	//				  << " " << currentNote.relativeBegin
	//				  << " " << currentNote.relativeEnd << std::endl;
	//
	//		currentScore.pop();
	//	}
}

void MainWindow::timeOutSlot()
{
	timeOut();
}

void
MainWindow::createMenus()
{

}

void
MainWindow::createToolBars()
{

}

void
MainWindow::createDockWindows()
{

}

void
MainWindow::initListeners()
{

}
