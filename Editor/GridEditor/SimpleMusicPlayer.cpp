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

	waveform_display = new QLabel();
	//permet de réduire la fenêtre:
	waveform_display->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

	QImage waveform(slideBar->width(), 50, QImage::Format_RGB32);
	waveform.fill(0);
	waveform_display->setPixmap(QPixmap::fromImage(waveform));

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
 * @brief SimpleMusicPlayer::resizeEvent
 * @param event QResizeEvent reçu de l'évènement
 *
 * Actualise le graphe en cas de redimensionnement de la fenêtre.
 */
void SimpleMusicPlayer::resizeEvent(QResizeEvent * event)
{
	if(player->getState())
	{
		updateWaveform();
	}
}

/**
 * @brief SimpleMusicPlayer::updateWaveform
 *
 * Actualise le graphe.
 */
void SimpleMusicPlayer::updateWaveform()
{
	unsigned const int size = slideBar->width() - 1;
	QImage waveform(size, 50, QImage::Format_RGB32);

	waveform.fill(0);
	for(int i = 0; i < size; i++)
		waveform.setPixel(i, 24, QColor(75, 200, 0).rgb());
	displayGraph(&waveform, size);

	waveform_display->setPixmap(QPixmap::fromImage(waveform));
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

	updateWaveform();
    return true;
}

/**
 * @brief SimpleMusicPlayer::displayGraph
 *
 * Affiche le sonogramme du morceau chargé.
 */
void SimpleMusicPlayer::displayGraph(QImage * waveform, unsigned int pixelWidth)
{
	int *table = new int[pixelWidth];
	unsigned int height = 0;
	player->getFullSpectrum(table, pixelWidth);

	for(int i = 0; i < pixelWidth ; i++)
	{
		//height = abs(table[i] / (int) pow(2, 23)); //moyenne
		height = abs(table[i] / (int) pow(2, 19));//max
		for(int j = 24 - height; j < 25 + height; j++)
		{
			waveform->setPixel(i, j, QColor(75, 200, 0).rgb());
		}
	}
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
