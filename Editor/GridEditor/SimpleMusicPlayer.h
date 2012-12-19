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
    QToolButton* stopButton;
    QToolButton* pauseButton;
    QToolButton* playButton;
    QSlider* slideBar;
    QLabel* timerLabel;
    QTimer* timer;

	QImage* waveform;
	QLabel *waveform_display;

    MusicPlayer* player;
    unsigned int songLength;
    unsigned int currentPosition;

public:
    SimpleMusicPlayer();
    ~SimpleMusicPlayer();
    QString getSong();
    bool setAudioFile(QString file);

	void displayGraph();

public slots:
    void play();
    void pause();
    void stop();
    void refreshTimerLabel();
    void updateSlideBar();
    void changePosition(int);
signals:
    void browseAudioFile();
    void audioFileDeleted();
};

#endif // SIMPLEMUSICPLAYER_H
