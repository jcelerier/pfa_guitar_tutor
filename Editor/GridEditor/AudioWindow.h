#ifndef AUDIOWINDOW_H
#define AUDIOWINDOW_H

#include <QWidget>
#include "AudioSync.h"
#include "SimpleMusicPlayer.h"

class AudioWindow : public QWidget
{
	Q_OBJECT

	public:
		explicit AudioWindow(QWidget *parent = 0);
		~AudioWindow();
		QString getSong();

	public slots:
		void browseAudioFile();
		void setAudioFile();
		void resetAudioFile();

	private slots:
		void refreshTimerAudioSync(int);

	private:
		SimpleMusicPlayer* player;
		QLabel* title;
		QLineEdit* audioFile;
		AudioSync* audioSync;
		QPushButton *browseButton;
		QGridLayout* layout;
};

#endif // AUDIOWINDOW_H
