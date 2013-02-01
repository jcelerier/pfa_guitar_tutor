#include "SimpleMusicPlayer.h"
#include <QDebug>
/**
 * @brief SimpleMusicPlayer::SimpleMusicPlayer
 *
 * Construit l'interface du lecteur audio.
 */
SimpleMusicPlayer::SimpleMusicPlayer()
{
	layout = new QGridLayout();
    playButton = new QToolButton();
    pauseButton = new QToolButton();
    stopButton = new QToolButton();
    slideBar = new QSlider(Qt::Horizontal);
    timerLabel = new QLabel("");
    timer = new QTimer();

	player = new MusicPlayer();
	waveform = new Waveform(this, slideBar->width(), 100);

    playButton->setIcon(QIcon("icons/play.png"));
    pauseButton->setIcon(QIcon("icons/pause.png"));
    stopButton->setIcon(QIcon("icons/stop.png"));

    layout->addWidget(slideBar, 0, 0, 1, 7);
    layout->addWidget(playButton, 1, 0);
    layout->addWidget(pauseButton, 1, 1);
    layout->addWidget(stopButton, 1, 2);
    layout->addWidget(timerLabel, 1, 6);
	layout->addWidget(waveform, 2, 0, 1, 7);

	QPushButton * plus = new QPushButton("+");
	QPushButton * moins = new QPushButton("-");
	QPushButton * gauche = new QPushButton("G");
	QPushButton * droite = new QPushButton("D");

	layout->addWidget(plus, 3, 0, 1, 1);
	layout->addWidget(moins, 4, 0, 1, 1);
	layout->addWidget(gauche, 3, 1, 1, 1);
	layout->addWidget(droite, 3, 2, 1, 1);

    setLayout(layout);

	connect(plus, SIGNAL(clicked()), this, SLOT(zoomIn()));
	connect(moins, SIGNAL(clicked()), this, SLOT(zoomOut()));

	connect(gauche, SIGNAL(clicked()), this, SLOT(moveLeft()));
	connect(droite, SIGNAL(clicked()), this, SLOT(moveRight()));

	connect(this, SIGNAL(sendTimers(QTime,QTime,QTime)), waveform, SLOT(setTimers(QTime,QTime,QTime)));

    connect(playButton, SIGNAL(released()), this, SLOT(play()));
    connect(pauseButton, SIGNAL(released()), this, SLOT(pause()));
    connect(stopButton, SIGNAL(released()), this, SLOT(stop()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSlideBar()));
    connect(slideBar, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));


    currentPosition = 0;
    songLength = 0;
	refreshTimerLabel();
}

/**
 * @brief SimpleMusicPlayer::~SimpleMusicPlayer
 */
SimpleMusicPlayer::~SimpleMusicPlayer()
{
    delete slideBar;
    delete layout;
    delete pauseButton;
    delete playButton;
    delete stopButton;
    delete timerLabel;
    delete player;
    delete timer;
}



/**
 * @brief SimpleMusicPlayer::getSong
 * @return Le chemin vers le fichier audio courant.
 *
 * Retourne le chemin vers le fichier audio courant.
 */
QString SimpleMusicPlayer::getSong()
{
    return player->getSong();
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
    if(!player->setSong(file)) {
        QMessageBox::information(this, tr("So sorry..."), tr("There was an error while trying to play the file..."));
        return false;
    }
    songLength = player->getTotalLength();
    refreshTimerLabel();
    slideBar->setRange(0, songLength);

	//player->getFullSpectrum(waveform->getSpectrum(), waveform->getWidth());
	waveBegin = 0;
	waveEnd = player->getTotalLengthInSamples();

	player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
	waveform->update();

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
	if(player->getState())
	{
		waveform->setWidth(slideBar->width());
		//player->getFullSpectrum(waveform->getSpectrum(), waveform->getWidth());

		player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
		waveform->update();
	}
}


/**
 * @brief SimpleMusicPlayer::getCurrentPosition
 * @return Donne la position actuelle dans le morceau.
 */
QTime SimpleMusicPlayer::getCurrentPosition()
{
    unsigned int time = player->getPosition();
    QTime t(time/(1000*60*60),time/(1000*60)%60, (time/1000)%60, time%1000);
    return t;
}

/**
 * @brief SimpleMusicPlayer::play
 *
 * Lecture du fichier audio choisi.
 */
void SimpleMusicPlayer::play()
{
    if(player->getState()) {
		player->play();
        timer->start(REFRESH_DELAY);
    }
    else
        emit browseAudioFile();
}

/**
 * @brief SimpleMusicPlayer::pause
 *
 * Mise en pause de la lecture.
 */
void SimpleMusicPlayer::pause()
{
    if(player->getState())
        player->pause();
}

/**
 * @brief SimpleMusicPlayer::stop
 *
 * Arrêt de la lecture.
 */
void SimpleMusicPlayer::stop()
{
    if(player->getState()) {
        player->stop();
        songLength = 0;
        currentPosition = 0;
        timer->stop();
        slideBar->setSliderPosition(0);
        refreshTimerLabel();
        emit audioFileDeleted();
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
    if(currentPosition > 0) {
        int min = (currentPosition/1000)/60;
        int sec = (currentPosition/1000)%60;
        if(sec<10)
            timerLabel->setText(QString(tmp1.setNum(min) + ":0" + tmp2.setNum(sec) + " / "));
        else
            timerLabel->setText(QString(tmp1.setNum(min) + ":" + tmp2.setNum(sec) + " / "));
    }
    else {
        if(player->getState())
            timerLabel->setText("0 / ");
        else
            timerLabel->setText(".. / ");
    }
    if(songLength > 0) {
        int min = (songLength/1000)/60;
        int sec = (songLength/1000)%60;
        if(sec<10)
            timerLabel->setText(QString(timerLabel->text() + tmp1.setNum(min) + ":0" + tmp2.setNum(sec)));
        else
            timerLabel->setText(QString(timerLabel->text() + tmp1.setNum(min) + ":" + tmp2.setNum(sec)));
    }
    else
        timerLabel->setText(QString(timerLabel->text() + ".."));
}

/**
 * @brief SimpleMusicPlayer::updateSlideBar
 *
 * Rafraîchissement de la barre de défilement.
 */
void SimpleMusicPlayer::updateSlideBar()
{
    currentPosition = player->getPosition();
    slideBar->setSliderPosition(currentPosition);
    refreshTimerLabel();
    timer->start(REFRESH_DELAY);
}

/**
 * @brief SimpleMusicPlayer::changePosition
 * @param position Nouvelle position exprimée en millisecondes
 *
 * Demande à aller à un temps précis dans le morceau courant.
 */
void SimpleMusicPlayer::changePosition(int position)
{
    player->changePosition(position);
	currentPosition = player->getPosition();
}


void SimpleMusicPlayer::zoomIn()
{
	int med = (waveEnd - waveBegin) / 20;
	waveBegin += med/2;
	waveEnd -= med/2;

	player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
	waveform->update();
}

void SimpleMusicPlayer::zoomOut()
{
	int med = (waveEnd - waveBegin) / 20;
	waveBegin = std::max(0, waveBegin - med);
	waveEnd = std::min((signed) player->getTotalLengthInSamples(), waveEnd + med);

	player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
	waveform->update();
}

void SimpleMusicPlayer::moveLeft()
{
	int mvt = (waveEnd - waveBegin) / waveform->getWidth() ;

	if(waveBegin < 0) { }
	else if(waveBegin - mvt >= 0)
	{
		waveBegin -= mvt;
		waveEnd -= mvt;
	}
	else
	{
		waveBegin = 0;
		waveEnd += waveBegin - mvt;
	}
	player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
	waveform->update();
}

void SimpleMusicPlayer::moveRight()
{
	int lgr = player->getTotalLengthInSamples();
	int l = waveEnd - waveBegin;
	int mvt = l / waveform->getWidth() ;

	if(waveEnd > lgr) { }
	else if (waveEnd + mvt <= lgr)
	{
		waveBegin += mvt;
		waveEnd += mvt;
	}
	else
	{
		waveBegin += waveEnd + mvt - lgr;
		waveEnd = lgr;
	}

	player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
	waveform->update();
}


int SimpleMusicPlayer::getWaveBegin()
{
	return waveBegin;
}

int SimpleMusicPlayer::getWaveEnd()
{
	return waveEnd;
}
