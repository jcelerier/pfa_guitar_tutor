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

	waveform = new QImage(600, 50, QImage::Format_RGB32);

	waveform_display = new QLabel();
	waveform->fill(0);


    playButton->setIcon(QIcon("icons/play.png"));
    pauseButton->setIcon(QIcon("icons/pause.png"));
    stopButton->setIcon(QIcon("icons/stop.png"));

    layout->addWidget(slideBar, 0, 0, 1, 7);
    layout->addWidget(playButton, 1, 0);
    layout->addWidget(pauseButton, 1, 1);
    layout->addWidget(stopButton, 1, 2);
    layout->addWidget(timerLabel, 1, 6);
	layout->addWidget(waveform_display, 2, 0, 1, 7);
    setLayout(layout);

    connect(playButton, SIGNAL(released()), this, SLOT(play()));
    connect(pauseButton, SIGNAL(released()), this, SLOT(pause()));
    connect(stopButton, SIGNAL(released()), this, SLOT(stop()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSlideBar()));
    connect(slideBar, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));

    player = new MusicPlayer();

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

	displayGraph();
    return true;
}

/**
 * @brief SimpleMusicPlayer::displayGraph
 *
 * Affiche le sonogramme du morceau chargé.
 */
void SimpleMusicPlayer::displayGraph()
{
	int width = 600; //ne pas mettre plus
	int *table = new int[width];
	int height = 0;
	player->getFullSpectrum(&table);

	for(int i = 0; i < width; i++)
	{
		height = abs(table[i] / (int) pow(2, 24));
		for(int j = 25 - height; j < 25 + height; j++ )
		{
			waveform->setPixel(i, j,  QColor(75, 200, 0).rgb());
		}
	}

	waveform_display->setPixmap(QPixmap::fromImage(*waveform));
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
}
