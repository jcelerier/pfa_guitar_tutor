#include "Configuration.h"
#include <QDebug>

Configuration::Configuration()
{

}

void Configuration::setSongName(QString songName)
{
	m_currentSongName = songName;
}

QString Configuration::getSongName()
{
	return m_currentSongName;
}

void Configuration::setAudioInput(QString audioInput)
{
	m_audioInput = audioInput;
}
QString Configuration::getAudioInput()
{
	return m_audioInput;
}

void Configuration::setAudioOutput(QString audioOutput)
{
	m_audioOutput = audioOutput;
}

QString Configuration::setAudioOutput()
{
	return m_audioOutput;
}
