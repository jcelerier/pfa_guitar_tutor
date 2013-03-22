#ifndef SIMPLEMUSICPLAYER_H
#define SIMPLEMUSICPLAYER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QTimer>
#include <QResizeEvent>
#include <QString>
#include <QPoint>
#include <QTime>
#include <MusicPlayer.h>
#include "Waveform.h"
#include "WaveformTimeBar.h"
#define REFRESH_DELAY 200
#define PLAYTIMER_DELAY 40

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
	QToolButton* playButton, *playBarButton;
    QSlider* slideBar;
    QLabel* timerLabel;
	QTimer* timer, *playTimer;

	Waveform *waveform;
	WaveformTimeBar* waveformTimeBar;

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

	void waveUpdate();

public slots:
    void play();
    void pause();
    void stop();
    void refreshTimerLabel();
    void updateSlideBar();
    void changePosition(int);

	void sendTimeData();

	void waveFullZoom();
	void waveSongZoom();
	void waveBarZoom();

	void zoomIn(QPoint);
	void zoomOut(QPoint);
	void moveLeft();
	void moveRight();

signals:
	void sigTimeData(QTime);
    void browseAudioFile();
    void audioFileDeleted();
	void sendTimers(QTime, QTime, QTime);
};

#endif // SIMPLEMUSICPLAYER_H
