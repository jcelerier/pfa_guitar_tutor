#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>
#include <portaudio.h>
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

	private:
		QString m_currentSongName;
		QString m_audioInput;
		QString m_audioOutput;
};

#endif // CONFIGURATION_H
