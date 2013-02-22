#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QtGui>
#include <QString>
#include <portaudio.h>

#define ORIGINAL_WIDTH 1920
#define ORIGINAL_HEIGHT 1080

class Configuration
{
	public:
		Configuration();

		void setSongName(QString songName);
		QString getSongName();

		void setAudioInput(QString audioInput);
		QString getAudioInput();

		void setAudioOutput(QString audioOutput);
		QString setAudioOutput();

        static QRect getWindowSize();

        static float quotient;
        static int height;
        static int width;
        static int const originalWidth=ORIGINAL_WIDTH;
        static int const originalHeight=ORIGINAL_HEIGHT;

	private:
		QString m_currentSongName;
		QString m_audioInput;
		QString m_audioOutput;



};

#endif // CONFIGURATION_H
