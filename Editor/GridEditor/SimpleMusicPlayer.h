#ifndef SIMPLEMUSICPLAYER_H
#define SIMPLEMUSICPLAYER_H

#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QResizeEvent>
#include <QString>
#include <QPoint>
#include <QTime>
#include <MusicPlayer.h>
#include "Waveform.h"
#define REFRESH_DELAY 200

/**
 * @brief Classe générant l'interface graphique d'un lecteur audio simple
 */
class SimpleMusicPlayer : public QWidget
{
    Q_OBJECT

private:
	QWidget* parent;
    QGridLayout* layout;
    QToolButton* stopButton;
    QToolButton* playButton;
    QSlider* slideBar;
    QLabel* timerLabel;
    QTimer* timer;

	Waveform *waveform;

    MusicPlayer* player;

    unsigned int songLength;
    unsigned int currentPosition;

	int waveBegin;
	int waveEnd;

protected:
    virtual void resizeEvent(QResizeEvent * event);

public:
	SimpleMusicPlayer(QWidget *parent);
    ~SimpleMusicPlayer();
    QString getSong();
    bool setAudioFile(QString file);

    QTime getCurrentPosition();

	int getWaveBegin();
	int getWaveEnd();

public slots:
    void play();
    void pause();
    void stop();
    void refreshTimerLabel();
    void updateSlideBar();
    void changePosition(int);

	void zoomIn(QPoint);
	void zoomOut(QPoint);
	void moveLeft();
	void moveRight();
signals:
    void browseAudioFile();
    void audioFileDeleted();
	void sendTimers(QTime, QTime, QTime);
};

#endif // SIMPLEMUSICPLAYER_H
