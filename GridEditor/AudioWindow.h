#ifndef AUDIOWINDOW_H
#define AUDIOWINDOW_H

#include <QtWidgets/QWidget>
#include <QString>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>

#include "AudioSync.h"
#include "ZoomButtons.h"
#include "SimpleMusicPlayer.h"
#include "WaveformTimeBar.h"

/**
 * @brief Tab qui contient toutes les informations relatives à l'audio
 */
class AudioWindow : public QWidget
{
	Q_OBJECT

	public:
		explicit AudioWindow(QWidget *parent = 0);
		~AudioWindow();
		QString getFilename();
		void setAudioFileName(QString file);
		void setWaveformData(Waveform* m_waveform, WaveformTimeBar* timeBar);
		void setBar(const int bar);
		void setBeginning(const int begin);
		void setEnd(const int end);
		int getBar();
		int getBeginning();
		int getEnd();


	public slots:
		void browseAudioFile();
		void setAudioFile();
		void resetAudioFile();
		void playFrom(int);

	private slots:
		void refreshTimerAudioSync(int);

	signals:
		void somethingChanged();
		void waveFullZoom();
		void waveBarZoom();
		void waveSongZoom();
		void timeSignatureChanged(int);

	private:
		SimpleMusicPlayer* m_player;
		QLabel* m_label;
		QLineEdit* m_audioFile;
		AudioSync* m_audioSync;
		QPushButton *m_browseButton;
		QGridLayout* m_layout;
		ZoomButtons* m_zoomButtons;

		Waveform* m_waveform;
		WaveformTimeBar* m_waveformTimeBar;
};

#endif // AUDIOWINDOW_H
