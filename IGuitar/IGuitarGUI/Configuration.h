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



        static int const framesPerSec=30;
        static int const originalWidth=ORIGINAL_WIDTH;
        static int const originalHeight=ORIGINAL_HEIGHT;

        static float getQuotient();
        static QRect getWindowSize();
        static void setWindowSize(QRect qr);
        static void setWindowSize(int w, int h);

	private:
		QString m_currentSongName;
		QString m_audioInput;
		QString m_audioOutput;

        static int height;
        static int width;
        static float quotient;


};

#endif // CONFIGURATION_H
