#ifndef AUDIOWINDOW_H
#define AUDIOWINDOW_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "AudioSync.h"
#include "ZoomButtons.h"
#include "SimpleMusicPlayer.h"

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
		void setWaveform(Waveform* waveform);

	public slots:
		void browseAudioFile();
		void setAudioFile();
		void resetAudioFile();
		void playFrom(int);

	private slots:
		void refreshTimerAudioSync(int);

	signals:
		void waveFullZoom();
		void waveBarZoom();
		void waveSongZoom();

	private:
		QWidget* parent;
		SimpleMusicPlayer* player;
        QLabel* label;
		QLineEdit* audioFile;
		AudioSync* audioSync;
		QPushButton *browseButton;
		QGridLayout* layout;
		Waveform* waveform;
};

#endif // AUDIOWINDOW_H
