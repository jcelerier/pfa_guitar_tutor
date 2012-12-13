#include "MusicPlayer.h"
#include <iostream>

MusicPlayer::MusicPlayer()
{
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    state = false;
}

MusicPlayer::MusicPlayer(QString song)
{
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    state = setSong(song);
}


MusicPlayer::~MusicPlayer()
{
    FMOD_Sound_Release(music);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}

void MusicPlayer::play()
{
    if(state) {
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, music, 0, &channel);
        FMOD_Channel_SetPaused(channel, 0);
    }
}

void MusicPlayer::pause()
{
    if(state) {
        FMOD_BOOL isPaused;
        FMOD_Channel_GetPaused(channel, &isPaused);

        if (isPaused)
            FMOD_Channel_SetPaused(channel, 0);
        else
            FMOD_Channel_SetPaused(channel, 1);
    }
}

void MusicPlayer::stop()
{
    if(state) {
        state = false;
        FMOD_Channel_Stop(channel);
    }
}

bool MusicPlayer::setSong(QString song)
{
    const char* tmp = song.toStdString().c_str();
    FMOD_RESULT result = FMOD_System_CreateSound(system, tmp, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &music);

    if (result != FMOD_OK) {
        state = false;
        return false;
    }
    this->song = song;
    state = true;
    return true;
}

QString MusicPlayer::getSong() const
{
    return song;
}

bool MusicPlayer::getState() const
{
    return state;
}

unsigned int MusicPlayer::getPosition() const
{
    if(state) {
        unsigned int tmp;
        if(FMOD_Channel_GetPosition(channel, &tmp, FMOD_TIMEUNIT_MS) == FMOD_OK)
            return tmp;
    }
    return 0;
}

unsigned int MusicPlayer::getTotalLength() const
{
    if(state) {
        unsigned int tmp = 0;
        if(FMOD_Sound_GetLength(music, &tmp, FMOD_TIMEUNIT_MS) == FMOD_OK)
            return tmp;
    }
    return 0;
}

void MusicPlayer::changePosition(unsigned int position)
{
    if(state)
        FMOD_Channel_SetPosition(channel, position, FMOD_TIMEUNIT_MS);
}
