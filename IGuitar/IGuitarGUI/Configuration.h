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

		void setInputIndex(int);
		void setOutputIndex(int);
		int getInputIndex();
		int getOutputIndex();


		//private, please ?
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

		PaDeviceIndex m_inputIndex, m_outputIndex;
        static int m_height;
        static int m_width;
        static float m_quotient;


};

#endif // CONFIGURATION_H
