#include "MainWindow.hpp"


#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>

#include <fstream>
#include <unistd.h>

extern "C" {
#include "chordfft/chord_fft.h"

static void
fill_buffer_fft(double *buffer, double *new_buffer)
{
  int i;
  double tmp[FRAME_SIZE_FFT-HOP_SIZE_FFT];

  /* save */
  for (i=0;i<FRAME_SIZE_FFT-HOP_SIZE_FFT;i++)
    tmp[i] = buffer[i+HOP_SIZE_FFT];

  /* save offset */
  for (i=0;i<(FRAME_SIZE_FFT-HOP_SIZE_FFT);i++)
    {
      buffer[i] = tmp[i];
    }

  for (i=0;i<HOP_SIZE_FFT;i++)
    {
      buffer[FRAME_SIZE_FFT-HOP_SIZE_FFT+i] = new_buffer[i];
    }
}

static int
read_n_samples_fft (SNDFILE * infile, double * buffer, int channels, int n)
{

  if (channels == 1)
    {
      /* MONO */
      int readcount ;

      readcount = sf_readf_double (infile, buffer, n);

      return readcount==n;
    }
  else if (channels == 2)
    {
      /* STEREO */
      double buf [2 * n] ;
      int readcount, k ;
      readcount = sf_readf_double (infile, buf, n);
      for (k = 0 ; k < readcount ; k++)
	buffer[k] = (buf [k * 2]+buf [k * 2+1])/2.0 ;


      return readcount==n;
    }
  else
    {
      /* FORMAT ERROR */
      printf ("Channel format error %d.\n", channels);
    }


  return 0;
}

static int
read_samples_fft (SNDFILE * infile, double * buffer, int channels)
{
  return read_n_samples_fft (infile, buffer, channels, HOP_SIZE_FFT);
}

int
saveChordsInFile (char* infilename, char * outfilename)
{
	SNDFILE	 	*infile = NULL ;
	FILE		*outfile = NULL ;
	SF_INFO	 	sfinfo ;


	if (strcmp (infilename, outfilename) == 0)
	{	printf ("Error : Input and output filenames are the same.\n\n") ;
		//print_usage (progname) ;
		return 1 ;
		} ;


	if ((infile = sf_open (infilename, SFM_READ, &sfinfo)) == NULL)
	{	printf ("Not able to open input file %s.\n", infilename) ;
		puts (sf_strerror (NULL)) ;
		return 1 ;
		} ;

	/* Open the output file. */
	if ((outfile = fopen (outfilename, "a")) == NULL)
	{	printf ("Not able to open output file %s : %s\n", outfilename, sf_strerror (NULL)) ;
		return 1 ;
		} ;

	//fprintf (outfile, "# Converted from file %s.\n", infilename) ;
 	//fprintf (outfile, "# Channels %d, Sample rate %d\n", sfinfo.channels, sfinfo.samplerate) ;

	printf("Processing : %s\n",infilename);

	/* verify sampling rate */
	printf("Sampling rate : %d\n",sfinfo.samplerate);
	printf("Channels : %d\n",sfinfo.channels);

	/* Read WAV and Compute Chromas */
	double buffer[FRAME_SIZE_FFT];
	double new_buffer[HOP_SIZE_FFT];

	int i;
	for (i=0;i<(FRAME_SIZE_FFT/HOP_SIZE_FFT-1);i++)
	  {
	    if (read_samples_fft (infile, new_buffer, sfinfo.channels)==1)
	      fill_buffer_fft(buffer, new_buffer);
	    else
	      {
		printf("not enough samples !!\n");
		return 1;
	      }
	  }


	chord_ctrl_fft *c = (chord_ctrl_fft*) malloc(sizeof(struct chord_ctrl_fft));
	chord_init_fft(c, sfinfo.samplerate, HOP_SIZE_FFT, FRAME_SIZE_FFT);

	while (read_samples_fft (infile, new_buffer, sfinfo.channels)==1)
	  {

	    /* Process Samples */


	    /* hop size */
	    fill_buffer_fft(buffer, new_buffer);


	    /* Process */
	    chroma_compute_fft(c, buffer, FRAME_SIZE_FFT);


	  }


	/* output chord */
	chord_compute_fft(c, outfile);
	/* exit */
	chord_exit_fft(c);

	sf_close (infile) ;
	fclose (outfile) ;

	return EXIT_SUCCESS ;
} /* main */
}

MainWindow::~MainWindow()
{

}

MainWindow::MainWindow()
: QWidget()
{  
	m_saveChordsComputation = false;
	m_playChordsComputation = false;
	m_stopChordsComputation = false;
	m_mustBeginRecord = false;

	m_playBackScoreManager = NULL;
	m_scoreManager = NULL;

	buildGUI();


}

void
MainWindow::setArgs(int argc, char* argv[])
{
	for (unsigned int i = 0; i < argc; i++) {
		m_commandLineArguments.push_back(argv[i]);
	}
}

void
MainWindow::saveChords()
{
	m_saveChordsComputation = true;
}

void
MainWindow::playRecord(bool mustPlayBack)
{

	if (m_playBackScoreManager == NULL) {
		m_playChordsComputation = true;
		m_mustPlayBack = mustPlayBack;
	}


	 // sleep(3);
}

void
MainWindow::stopRecord()
{
	m_stopChordsComputation = true;
}

void
MainWindow::startRecord()
{
	m_mustBeginRecord = true;
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
	if (m_mustBeginRecord) {
		m_mustBeginRecord = false;

		MusicManager* musicManager;

		if (m_commandLineArguments.size() >= 2) {
			std::map<std::string, std::string> multiTracksMap;
			std::vector<std::string> muteTracks;

			multiTracksMap["all"] = m_commandLineArguments[1];

			musicManager = new MusicManager(multiTracksMap, muteTracks);

		} else {
			musicManager = new MusicManager(TIME_MAX_TO_RECORD);
		}

		m_scoreManager = new ScoreManager(musicManager);

		if (m_commandLineArguments.size() >= 3) {
			m_scoreManager->loadScore(m_commandLineArguments[2]);
		}

		m_scoreManager->run();

		usleep(100000);

		//std::cout << "FIN" << std::endl;
		m_renderAreas.setStopRecordButtonVisible();
	}

	if (m_scoreManager != NULL) {
		if (m_scoreManager->isRunning()) {
			prioritizedScore currentScore;
			m_scoreManager->getScore(currentScore, MIN_SCORE_BOUND, MAX_SCORE_BOUND);

			m_scoreManager->update();

			bool mustGoToTheNextPart = (m_scoreManager->getValidatedNotesPercent() >= PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART);
			if(mustGoToTheNextPart) {
				m_scoreManager->setToNaturalNextPart();
			}

			m_renderAreas.drawScore(currentScore, m_scoreManager->getCurrentChord(), m_scoreManager->getCurrentPart(), m_scoreManager->getNextPart(), mustGoToTheNextPart);
		}
	}

	if (m_saveChordsComputation) {
		m_saveChordsComputation = false;

		m_scoreManager->stop();
		//sleep(1);

		//std::cout << "Will save recorded data" << std::endl;
		m_scoreManager->saveRecordedData(INPUT_SAVE_LOCATION);

		//std::cout << "Will save INTRO" << std::endl;
		std::ofstream myfile;
		myfile.open (INPUT_CHORD_SAVE_LOCATION);
		myfile << "[INTRO]\n";
		myfile.close();
		//std::cout << "INTRO saved" << std::endl;

		saveChordsInFile(INPUT_SAVE_LOCATION, INPUT_CHORD_SAVE_LOCATION);

		m_renderAreas.createPlayStopButtons();

		//m_scoreManager->stop();
		m_scoreManager = NULL;

	} else if (m_playChordsComputation) {
		m_playChordsComputation = false;



		std::map<std::string, std::string> multiTracksMap;
		std::vector<std::string> muteTracks;

		multiTracksMap["all"] = INPUT_SAVE_LOCATION;
		//multiTracksMap["all"] = "Tracks/BeatlesDayInTheLife/AllTracks.wav";

		if (!m_mustPlayBack) {
			muteTracks.push_back("all");
		}

		m_musicManager = new MusicManager(multiTracksMap, muteTracks);
		m_playBackScoreManager = new ScoreManager(m_musicManager);

		m_playBackScoreManager->loadScore(INPUT_CHORD_SAVE_LOCATION);

		m_playBackScoreManager->run();

		usleep(100000);

	//	m_playBackScoreManager->setNextPart(SCORE_END);




	} else if (m_stopChordsComputation) {
		m_stopChordsComputation = false;

		if (m_playBackScoreManager != NULL) {

			m_playBackScoreManager->stop();

			//delete m_playBackScoreManager;
			//delete m_musicManager;

			m_playBackScoreManager = NULL;
		}
	}

	if (m_playBackScoreManager != NULL) {
		if (m_playBackScoreManager->isRunning()) {
			if (m_playBackScoreManager->getCurrentPart() == SCORE_END) {
				//std::cout << "JE L'ARRETE" << std::endl;
				m_playBackScoreManager->stop();
			} else {
				prioritizedScore currentScore;

				m_playBackScoreManager->getScore(currentScore, MIN_SCORE_BOUND, MAX_SCORE_BOUND);

				m_playBackScoreManager->update();


				//			bool mustGoToTheNextPart = (m_scoreManager->getValidatedNotesPercent() >= PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART);
				//			if(mustGoToTheNextPart) {
				//				m_scoreManager->setToNaturalNextPart();
				//			}


				m_renderAreas.drawScore(currentScore, m_playBackScoreManager->getCurrentChord(), " ", "Playing Back", false);

			}

		}
	}




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
