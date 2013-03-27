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
	QGridLayout* m_layout;
	QToolButton* m_stopButton;
	QToolButton* m_playButton, *m_playBarButton;
	QSlider* m_slideBar;
	QLabel* m_timerLabel;
	QTimer* m_timer, *m_playTimer;

	Waveform *m_waveform;
	WaveformTimeBar* m_waveformTimeBar;

	MusicPlayer* m_player;

	unsigned int m_songLength;
	unsigned int m_currentPosition;

	int m_waveBegin;
	int m_waveEnd;

protected:
	virtual void resizeEvent(QResizeEvent * event);

public:
	SimpleMusicPlayer(QWidget *parent);
	~SimpleMusicPlayer();
	QString getSong();
	bool setAudioFile(QString);

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
