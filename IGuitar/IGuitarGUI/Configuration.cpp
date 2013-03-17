#include "Configuration.h"
#include <QDebug>

int Configuration::m_width = 1366;
int Configuration::m_height = 768;

/*
int Configuration::width = 1920;
int Configuration::height = 1080;
*/
float Configuration::m_quotient = float(Configuration::m_height)/float(Configuration::originalHeight); // L'image de base est de hauteur 1080, pour mise a l'echelle

Configuration::Configuration()
{
	m_inputIndex = 0;
	m_outputIndex = 0;
}

float Configuration::getQuotient()
{
	return Configuration::m_quotient;
}


QRect Configuration::getWindowSize()
{
	return QRect(0, 0, Configuration::m_width, Configuration::m_height);
}

void Configuration::setWindowSize(int w, int h)
{
   Configuration::m_width = w;
   Configuration::m_height = h;
   Configuration::m_quotient = float(Configuration::m_height)/float(Configuration::originalHeight);
}

void Configuration::setWindowSize(QRect qr)
{
   Configuration::m_width = qr.width();
   Configuration::m_height = qr.height();
   Configuration::m_quotient = float(Configuration::m_height)/float(Configuration::originalHeight);
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

int Configuration::getInputIndex()
{
	return m_inputIndex;
}

int Configuration::getOutputIndex()
{
	return m_outputIndex;
}


void Configuration::setInputIndex(int i)
{
	m_inputIndex = i;
}

void Configuration::setOutputIndex(int i)
{
	m_outputIndex = i;
}
