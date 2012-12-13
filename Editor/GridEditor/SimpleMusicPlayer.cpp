#include "SimpleMusicPlayer.h"

SimpleMusicPlayer::SimpleMusicPlayer()
{
    layout = new QGridLayout();
    QGridLayout *internLayout = new QGridLayout();
    browseButton = new QPushButton(tr("Browse"));
    playButton = new QToolButton();
    pauseButton = new QToolButton();
    stopButton = new QToolButton();
    slideBar = new QSlider(Qt::Horizontal);
    timerLabel = new QLabel("");
    timer = new QTimer();

    playButton->setIcon(QIcon("icons/play.png"));
    pauseButton->setIcon(QIcon("icons/pause.png"));
    stopButton->setIcon(QIcon("icons/stop.png"));

    internLayout->addWidget(slideBar, 0, 0, 1, 7);
    internLayout->addWidget(playButton, 1, 0);
    internLayout->addWidget(pauseButton, 1, 1);
    internLayout->addWidget(stopButton, 1, 2);
    internLayout->addWidget(timerLabel, 1, 6);
    layout->addLayout(internLayout, 0, 0, 1, 3);
    layout->addWidget(browseButton, 0, 3);

    setLayout(layout);

    connect(browseButton, SIGNAL(released()), this, SLOT(browseFile()));
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

SimpleMusicPlayer::~SimpleMusicPlayer()
{
    delete slideBar;
    delete browseButton;
    delete layout;
    delete pauseButton;
    delete playButton;
    delete stopButton;
    delete timerLabel;
    delete player;
    delete timer;
}

void SimpleMusicPlayer::browseFile()
{
    QString tmp = QFileDialog::getOpenFileName(this, tr("Open a file"), QString(), tr("Music (*.mp3 *.ogg *.wma *.wav)"));
    if(tmp != "") {
        if(!player->setSong(tmp)) {
            QMessageBox::information(this, tr("So sorry..."), tr("There was an error while trying to play the file..."));
            return;
        }
        songLength = player->getTotalLength();
        refreshTimerLabel();
        slideBar->setRange(0, songLength);
    }
}

void SimpleMusicPlayer::play()
{
    if(player->getState()) {
        player->play();
        timer->start(REFRESH_DELAY);
    }
    else
        browseFile();
}

void SimpleMusicPlayer::pause()
{
    if(player->getState())
        player->pause();
}

void SimpleMusicPlayer::stop()
{
    if(player->getState()) {
        player->stop();
        songLength = 0;
        currentPosition = 0;
        timer->stop();
        slideBar->setSliderPosition(0);
        refreshTimerLabel();
    }
}

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

void SimpleMusicPlayer::updateSlideBar()
{
    currentPosition = player->getPosition();
    slideBar->setSliderPosition(currentPosition);
    refreshTimerLabel();
    timer->start(REFRESH_DELAY);
}

void SimpleMusicPlayer::changePosition(int position)
{
    player->changePosition(position);
}
