#include "SimpleMusicPlayer.h"
#include "AudioWindow.h"
#include <Util.hpp>
#include <QtWidgets/QMessageBox>




//trucs pour le zoom
#define MOVE_FACTOR 3
#define ZOOM_FACTOR 20

//nécessaire pour emplir l'espace, don't ask me why.. Sans doute à cause des 9 pixels de marge de chaque côté
#define WIDTH_ADJUSTMENT 18
/**
 * @brief SimpleMusicPlayer::SimpleMusicPlayer
 *
 * Construit l'interface du lecteur audio.
 */
SimpleMusicPlayer::SimpleMusicPlayer(QWidget* parent) : QWidget(parent)
{
	this->m_parent = parent;
	m_layout = new QGridLayout();
	m_playButton = new QToolButton();
	// A FAIRE : playBarButton = new QToolButton();
	m_stopButton = new QToolButton();
	m_slideBar = new QSlider(Qt::Horizontal);
	m_timerLabel = new QLabel("");
	m_timer = new QTimer();
	m_playTimer = new QTimer();


	m_player = new MusicPlayer();

	m_waveform = new Waveform(this, ((AudioWindow*) parent)->width()  - WIDTH_ADJUSTMENT, 300);
	m_waveformTimeBar = new WaveformTimeBar(QTime(0, 0), this);
    ((AudioWindow*) parent)->setWaveformData(m_waveform, m_waveformTimeBar);


	m_playButton->setIcon(QIcon(":/icons/play.png"));
	//playBarButton->setIcon(QIcon(":/icons/play.png"));
	m_stopButton->setIcon(QIcon(":/icons/stop.png"));

	m_layout->addWidget(m_slideBar, 0, 0, 1, 7);
	m_layout->addWidget(m_playButton, 1, 0);
	//layout->addWidget(playBarButton, 1, 1);
	m_layout->addWidget(m_stopButton, 1, 2);
	m_layout->addWidget(m_timerLabel, 1, 6);

	setLayout(m_layout);

	m_waveform->setWidth(parent->width() - WIDTH_ADJUSTMENT);
	m_waveform->initImage();
	m_waveform->update();

	connect(m_playButton, SIGNAL(released()), this, SLOT(pause()));
	//connect(playBarButton, SIGNAL(clicked()), this, SLOT(playBar()));
	connect(m_stopButton, SIGNAL(released()), this, SLOT(stop()));
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideBar()));
	connect(m_playTimer, SIGNAL(timeout()), this, SLOT(sendTimeData()));

    connect(this, SIGNAL(sigTimeData(QTime)), m_waveform, SLOT(setPlayerTimer(QTime)));
    connect(this, SIGNAL(sigTimeData(QTime)), m_waveformTimeBar, SLOT(setPlayerTimer(QTime)));
    connect(m_waveformTimeBar, SIGNAL(playSliderModified(int)), this, SLOT(changePosition(int)));

	connect(m_slideBar, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));

	m_currentPosition = 0;
	m_songLength = 0;
	refreshTimerLabel();
}

/**
 * @brief SimpleMusicPlayer::~SimpleMusicPlayer
 */
SimpleMusicPlayer::~SimpleMusicPlayer()
{
	delete m_slideBar;
	delete m_layout;
	delete m_playButton;
	// delete playBarButton;
	delete m_playTimer;
	delete m_player;
	delete m_waveform;
	delete m_waveformTimeBar;
	delete m_stopButton;
	delete m_timerLabel;
	delete m_timer;
}



/**
 * @brief SimpleMusicPlayer::getSong
 * @return Le chemin vers le fichier audio courant.
 *
 * Retourne le chemin vers le fichier audio courant.
 */
QString SimpleMusicPlayer::getSong()
{
	return m_player->getSong();
}


/**
 * @brief SimpleMusicPlayer::setAudioFile
 * @param file Chemin vers le fichier audio.
 * @return Vrai si et seulement si le fichier audio a été ouvert correctement.
 *
 * Ouvre un nouveau fichier audio pour la lecture.
 */
bool SimpleMusicPlayer::setAudioFile(QString file)
{
	if(file == ""){
		return false;
	}

	if(!m_player->setSong(file)) {
		QMessageBox::information(this, tr("So sorry..."), QString(tr("Impossible to open the file ") + file));
		return false;
	}
	m_songLength = m_player->getTotalLength();
	refreshTimerLabel();
	m_slideBar->setRange(0, m_songLength);

	m_waveBegin = 0;
	m_waveEnd = m_player->getTotalLengthInSamples();

	m_waveform->setWidth(m_parent->width() - WIDTH_ADJUSTMENT);
	m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
	m_waveform->activate();
	waveUpdate();


	m_player->play();
	m_player->pause(true);
	m_timer->start(REFRESH_DELAY);

	return true;
}

/**
 * @brief SimpleMusicPlayer::resizeEvent
 * @param event QResizeEvent reçu de l'évènement
 *
 * Actualise le graphe en cas de redimensionnement de la fenêtre.
 */
void SimpleMusicPlayer::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	if(m_player->getState())
	{
		m_waveform->setWidth(m_parent->width() - WIDTH_ADJUSTMENT);

		m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());

	}
	else
	{
		m_waveform->setWidth(m_parent->width() - WIDTH_ADJUSTMENT);
	}
	waveUpdate();
}


/**
 * @brief SimpleMusicPlayer::getCurrentPosition
 * @return Donne la position actuelle dans le morceau.
 */
QTime SimpleMusicPlayer::getCurrentPosition()
{
	return MsecToTime(m_player->getPosition());
}

/**
 * @brief SimpleMusicPlayer::play
 *
 * Lecture du fichier audio choisi.
 */
void SimpleMusicPlayer::play()
{
	if(m_player->getState())
	{
		m_player->pause(false);
		m_playTimer->start(PLAYTIMER_DELAY);
	}
	else
		emit browseAudioFile();
}

/**
 * @brief SimpleMusicPlayer::sendTimeData
 *
 * Envoie le temps de lecture actuel
 */
void SimpleMusicPlayer::sendTimeData()
{
	emit sigTimeData(getCurrentPosition());
}

/**
 * @brief SimpleMusicPlayer::pause
 *
 * Mise en pause de la lecture.
 */
void SimpleMusicPlayer::pause()
{
	if(m_player->getState())
	{

		if(m_player->isPaused())
		{
			m_player->pause(false);
			m_playTimer->start(PLAYTIMER_DELAY);
		}
		else
		{
			m_player->pause(true);
			m_playTimer->stop();
		}


		if(m_player->isPaused())
			m_playButton->setIcon(QIcon(":/icons/pause.png"));
		else
			m_playButton->setIcon(QIcon(":/icons/play.png"));
	}
}

/**
 * @brief SimpleMusicPlayer::stop
 *
 * Arrêt de la lecture.
 */
void SimpleMusicPlayer::stop()
{
	if(m_player->getState()) {
		m_player->stop();
		m_playTimer->stop();
		m_currentPosition = 0;
		m_slideBar->setSliderPosition(0);
        this->sendTimeData();
		refreshTimerLabel();
		m_playButton->setIcon(QIcon(":/icons/play.png"));
	}
}

/**
 * @brief SimpleMusicPlayer::refreshTimerLabel
 *
 * Rafraîchissement de l'affichage du chronomètre du lecteur.
 */
void SimpleMusicPlayer::refreshTimerLabel()
{
	QString tmp1, tmp2;
	if(m_currentPosition > 0) {
		int min = (m_currentPosition/1000)/60;
		int sec = (m_currentPosition/1000)%60;
		if(sec<10)
			m_timerLabel->setText(QString(tmp1.setNum(min) + ":0" + tmp2.setNum(sec) + " / "));
		else
			m_timerLabel->setText(QString(tmp1.setNum(min) + ":" + tmp2.setNum(sec) + " / "));
	}
	else {
		if(m_player->getState())
			m_timerLabel->setText("0:00 / ");
		else
			m_timerLabel->setText(".. / ");
	}
	if(m_songLength > 0) {
		int min = (m_songLength/1000)/60;
		int sec = (m_songLength/1000)%60;
		if(sec<10)
			m_timerLabel->setText(QString(m_timerLabel->text() + tmp1.setNum(min) + ":0" + tmp2.setNum(sec)));
		else
			m_timerLabel->setText(QString(m_timerLabel->text() + tmp1.setNum(min) + ":" + tmp2.setNum(sec)));
	}
	else
		m_timerLabel->setText(QString(m_timerLabel->text() + ".."));
}

/**
 * @brief SimpleMusicPlayer::updateSlideBar
 *
 * Rafraîchissement de la barre de défilement.
 */
void SimpleMusicPlayer::updateSlideBar()
{
	m_currentPosition = m_player->getPosition();
	m_slideBar->setSliderPosition(m_currentPosition);
    this->sendTimeData();
	refreshTimerLabel();
	m_timer->start(REFRESH_DELAY);
}

/**
 * @brief SimpleMusicPlayer::changePosition
 * @param position Nouvelle position exprimée en millisecondes
 *
 * Demande à aller à un temps précis dans le morceau courant.
 */
void SimpleMusicPlayer::changePosition(int position)
{
	m_player->changePosition(position);
	m_currentPosition = m_player->getPosition();
}


/**
 * @brief SimpleMusicPlayer::zoomIn
 * @param clickPos position du pointeur
 *
 * Effectue un zoom avant sur la waveform.
 * Algorithme : on augmente les positions gauche et droite de zoomFactor%
 *			relativement à la position du clic
 */
void SimpleMusicPlayer::zoomIn(QPoint clickPos)
{
	float clickPercent = (float) clickPos.x() / (float) m_waveform->getWidth();
	float sample = clickPercent * (m_waveEnd - m_waveBegin) + m_waveBegin;
	const int zoomFactor = ZOOM_FACTOR;

	if(m_waveEnd - m_waveBegin > 10000)
	{
		m_waveBegin += (sample - (float) m_waveBegin) / (float) zoomFactor;
		m_waveEnd -= (m_waveEnd - sample) / zoomFactor;

		if(m_waveEnd < m_waveBegin + 10000 || m_waveEnd < 0)
			m_waveEnd = m_waveBegin + 10000;

		m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
	}
	else
	{
		m_waveEnd = m_waveBegin + 10000;
	}
	waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::zoomOut
 * @param clickPos Position du clic
 *
 * Effectue un zoom arrière.
 */
void SimpleMusicPlayer::zoomOut(QPoint clickPos)
{
	float clickPercent = (float) clickPos.x() / (float) m_waveform->getWidth();
	float sample = clickPercent * (m_waveEnd - m_waveBegin) + m_waveBegin;
	const int zoomFactor = ZOOM_FACTOR;

	m_waveBegin = qMax(0,
						 qMin(m_waveBegin - (int) ((sample - (float) m_waveBegin) / (float) zoomFactor),
								  (int) m_player->getTotalLengthInSamples() - 10000));
	m_waveEnd = qMax(m_waveBegin + 10000,
					   qMin((signed) m_player->getTotalLengthInSamples(),
								m_waveEnd + qMax((int) ((m_waveEnd - sample) / zoomFactor),
												   0)));

	m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
	waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::moveLeft
 *
 * Effectue un déplacement à gauche.
 */
void SimpleMusicPlayer::moveLeft()
{
	int mvt = (m_waveEnd - m_waveBegin) * MOVE_FACTOR / m_waveform->getWidth() ;

	if(m_waveBegin > 0)
	{
		if(m_waveBegin - mvt >= 0) // cas valide
		{
			m_waveBegin -= mvt;
			m_waveEnd -= mvt;
		}
		else if(m_waveBegin - mvt < 0)// cas ou on est au bord
		{
			m_waveBegin = 0;
		}
	}

	m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
	waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::moveRight
 *
 * Effectue un déplacement à droite.
 */
void SimpleMusicPlayer::moveRight()
{
	int lgr = m_player->getTotalLengthInSamples();

	int mvt = (m_waveEnd - m_waveBegin) * MOVE_FACTOR / m_waveform->getWidth() ;

	if(m_waveEnd < lgr)
	{
		if (m_waveEnd + mvt <= lgr)
		{
			m_waveBegin += mvt;
			m_waveEnd += mvt;
		}
		else
		{
			m_waveEnd = lgr;
		}
	}

	m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
	waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::getWaveBegin
 * @return Sample gauche de la waveform
 */
int SimpleMusicPlayer::getWaveBegin()
{
	return m_waveBegin;
}

/**
 * @brief SimpleMusicPlayer::getWaveEnd
 * @return Sample droite de la waveform
 */
int SimpleMusicPlayer::getWaveEnd()
{
	return m_waveEnd;
}


/**
 * @brief SimpleMusicPlayer::waveFullZoom
 *
 * Zoom de manière à ce qu'on voie l'intégralité du fichier
 */
void SimpleMusicPlayer::waveFullZoom()
{
	m_waveBegin = 0;
	m_waveEnd = m_player->getTotalLengthInSamples();
	m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
	waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::waveSongZoom
 *
 * Zoom de manière à ce qu'on voie le morceau tel qu'il a été défini dans AudioSync
 */

void SimpleMusicPlayer::waveSongZoom()
{
	if(m_waveform->getSampleBegin() + 10000 < m_waveform->getSampleEnd())
	{
		m_waveBegin = m_waveform->getSampleBegin();
		m_waveEnd = m_waveform->getSampleEnd();
		m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
		waveUpdate();
	}
}
/**
 * @brief SimpleMusicPlayer::waveBarZoom
 *
 * Définit le zoom de manière à ce qu'on voie la première mesure
 */
void SimpleMusicPlayer::waveBarZoom()
{
	if(m_waveform->getSampleBegin() + 10000 < m_waveform->getSampleBar())
	{
		m_waveBegin = m_waveform->getSampleBegin();
		m_waveEnd = m_waveform->getSampleBar();
		m_player->getSpectrum(m_waveBegin, m_waveEnd, m_waveform->getSpectrum(), m_waveform->getWidth());
		waveUpdate();
	}
}

/**
 * @brief SimpleMusicPlayer::waveUpdate
 *
 * Mise à jour de la forme d'onde.
 */
void SimpleMusicPlayer::waveUpdate()
{
	m_waveform->update();
	m_waveformTimeBar->update();
}
