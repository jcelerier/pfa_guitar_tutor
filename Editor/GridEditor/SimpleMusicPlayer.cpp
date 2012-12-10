#include "SimpleMusicPlayer.h"

SimpleMusicPlayer::SimpleMusicPlayer()
{
    layout = new QGridLayout();
    browseButton = new QPushButton(tr("Browse"));
    playButton = new QToolButton();
    pauseButton = new QToolButton();
    stopButton = new QToolButton();
    slideBar = new QSlider(Qt::Horizontal);

    layout->addWidget(slideBar, 0, 0, 1, 3);
    layout->addWidget(browseButton, 0, 3, 1, 1);
    layout->addWidget(playButton, 1, 0);
    layout->addWidget(pauseButton, 1, 1);
    layout->addWidget(stopButton, 1, 2);
    setLayout(layout);

    connect(browseButton, SIGNAL(released()), this, SLOT(browseFile()));
    connect(playButton, SIGNAL(released()), this, SLOT(play()));
    connect(pauseButton, SIGNAL(released()), this, SLOT(pause()));
    connect(stopButton, SIGNAL(released()), this, SLOT(stop()));

    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
}

SimpleMusicPlayer::~SimpleMusicPlayer()
{
    delete slideBar;
    delete browseButton;
    delete layout;
    delete pauseButton;
    delete playButton;
    delete stopButton;
    FMOD_Sound_Release(music);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}

void SimpleMusicPlayer::browseFile()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open a file"), QString(), tr("Music (*.mp3 *.ogg *.wma *.wav)"));
    if(fileName != "") {
        const char* tmp = fileName.toStdString().c_str();
        FMOD_RESULT result = FMOD_System_CreateSound(system, tmp, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &music);

        if (result != FMOD_OK) {
            QMessageBox::information(this, tr("So sorry..."), tr("There was an error while trying to play the file..."));
            fileName = "";
            FMOD_Sound_Release(music);
            return;
        }
    }
}

void SimpleMusicPlayer::play()
{
    if(fileName != "")
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, music, 0, NULL);
    else
        browseFile();
}

void SimpleMusicPlayer::pause()
{
    if(fileName != "") {
        FMOD_CHANNELGROUP *channel;
        FMOD_BOOL state;
        FMOD_System_GetMasterChannelGroup(system, &channel);
        FMOD_ChannelGroup_GetPaused(channel, &state);

        if (state)
            FMOD_ChannelGroup_SetPaused(channel, 0);
        else
            FMOD_ChannelGroup_SetPaused(channel, 1);
    }
}

void SimpleMusicPlayer::stop()
{
    if(fileName != "") {
        fileName = "";
        FMOD_Sound_Release(music);
    }
}
