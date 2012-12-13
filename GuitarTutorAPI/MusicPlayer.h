#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <fmodex/fmod.h>
#include <QString>

class MusicPlayer
{
    bool state;
    QString song;
    FMOD_SYSTEM *system;
    FMOD_SOUND *music;
    FMOD_CHANNEL *channel;
public:
    MusicPlayer();
    MusicPlayer(QString);
    ~MusicPlayer();
    void play();
    void pause();
    void stop();
    bool setSong(QString);
    QString getSong() const;
    bool getState() const;
    unsigned int getPosition() const;
    unsigned int getTotalLength() const;
    void changePosition(unsigned int position);
};

#endif // MUSICPLAYER_H
