#ifndef SIMPLEMUSICPLAYER_H
#define SIMPLEMUSICPLAYER_H


#include <QtGui>
#include <fmodex/fmod.h>

class SimpleMusicPlayer : public QWidget
{
    Q_OBJECT

private:
    QGridLayout* layout;
    QPushButton* browseButton;
    QToolButton* stopButton;
    QToolButton* pauseButton;
    QToolButton* playButton;
    QSlider* slideBar;
    QString fileName;

    FMOD_SYSTEM *system;
    FMOD_SOUND *music;

public:
    SimpleMusicPlayer();
    ~SimpleMusicPlayer();

public slots:
    void browseFile();
    void play();
    void pause();
    void stop();
};

#endif // SIMPLEMUSICPLAYER_H
