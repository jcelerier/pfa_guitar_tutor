#ifndef SIMPLEMUSICPLAYER_H
#define SIMPLEMUSICPLAYER_H


#include <QtGui>
#include <MusicPlayer.h>

#define REFRESH_DELAY 200

/**
 * @brief Classe générant l'interface graphique d'un lecteur audio simple
 */
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
    QLabel* timerLabel;
    QTimer* timer;

    MusicPlayer* player;
    unsigned int songLength;
    unsigned int currentPosition;

public:
    SimpleMusicPlayer();
    ~SimpleMusicPlayer();
public slots:
    void browseFile();
    void play();
    void pause();
    void stop();
    void refreshTimerLabel();
    void updateSlideBar();
    void changePosition(int);
};

#endif // SIMPLEMUSICPLAYER_H
