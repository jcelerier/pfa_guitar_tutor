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
//    delete m_audioConfiguration;
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
//	m_audioConfiguration = new AudioConfiguration(m_configuration, (QWidget*) this);

	restartEngine();
}

/**
 * @brief Controler::ticTac
 *
 * Slot appelé à intervalle régulier pour permettre la mise à jour de l'interface et du scoreManager
 */
void Controler::ticTac() {
    if(m_scoreManager->isRunning()) {
		m_scene->updateScene();
        m_scoreManager->update();
        QString playedChord = QString(m_scoreManager->getCurrentChord().c_str());
        m_scene->setPlayedChord(playedChord);
    }
}

bool Controler::initSong()
{

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
	//m_audioConfiguration->show();
}

void Controler::startSong()
{
	qDebug() << "startSong";
	m_scoreManager->run();
	qDebug() << "run";


	// nécessaire pour pas que ça crash, pourquoi ? (jm)
	usleep(100000);

    m_scoreManager->setNextPart(m_track->getPartName(2).toStdString());
    m_timer->start(1000/Configuration::framesPerSec);
	//m_renderAreas.changeButtonMode(false);
}

void Controler::stopSong()
{
	qDebug() << "stopSong";
    m_timer->stop();

	if (m_scoreManager != 0)
	{
		m_scoreManager->stop();
		//m_scoreManager->goToInMs(0);
	}
}

/**** CETTE FONCTION EST A GARDER POUR VOIR COMMENT LES PARTIES SONT GEREES ****/
// c'était la fonction Timeout*


//	if (m_scoreManager != NULL)
//	{
//		prioritizedScore currentScore;

//		m_scoreManager->getScore(currentScore, MIN_SCORE_BOUND, MAX_SCORE_BOUND);
//		m_scoreManager->update();

//		bool mustGoToTheNextPart = (m_scoreManager->getValidatedNotesPercent() >= PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART);
//		if(mustGoToTheNextPart)
//		{
//			m_scoreManager->setToNaturalNextPart();
//		}

//		/*m_renderAreas.drawScore(currentScore,
//								m_scoreManager->getCurrentChord(),
//								m_scoreManager->getCurrentPart(),
//								m_scoreManager->getNextPart(),
//								mustGoToTheNextPart);*/
//	}

/********************************************************************************/


// amoi

/**
 * @brief Controler::elapsedTime
 * @return Le temps écoulé depuis la lecture du morceau
 *
 * Donne le temps écoulé depuis le début de la lecture du morceau.
 */
int Controler::elapsedTime()
{
	return clockOffset + globalClock.elapsed();
}

void Controler::startClock()
{
	if(!m_playing)
	{
		startSong();
		globalClock.start();
		m_playing=true;
	}
	else
	{
		stopSong();
		m_playing=false;
//		restartEngine(); // à remplacer par une fonction qui fait juste redémarrer le morceau
	}
}

void Controler::pauseClock()
{
    clockOffset += globalClock.elapsed();
}

/**
 * @brief Controler::getChordList
 * @return La liste des accords
 *
 * Accesseur pour la liste des accords du morceau.
 */
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
    m_timer->stop();
	clockOffset = 0;

	m_playing = false;

	if(!initSong())
	{
		exit(0);
		// note : ne pas appeler les méthodes de qApp (quit, exit...) car qApp->exec() n'est pas encore appelé
	}

    chordList = getChordList(m_track);

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
