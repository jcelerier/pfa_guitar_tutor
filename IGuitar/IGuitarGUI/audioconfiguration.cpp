#include "audioconfiguration.h"
#include "ui_audioconfiguration.h"

#include <string>
#include <QString>
#include <MusicManager.h>
#include <ScoreManager.h>
#include <iostream>
#include "Configuration.h"
#include "Controler.hpp"

AudioConfiguration::AudioConfiguration(Configuration* conf, QWidget *parent) :
	ui(new Ui::AudioConfiguration), m_conf(conf)
{
	m_parent = parent;
	ui->setupUi(this);
	init();
}

void AudioConfiguration::init()
{
	std::vector <std::string> OutputVect;
	QList <QString> outputList;

	std::vector <std::string> InputVect;
	QList <QString> inputList;

	Pa_Initialize();
	sendInputVect(InputVect);
	sendOutputVect(OutputVect);
	Pa_Terminate();

	for(unsigned int i = 0; i < OutputVect.size(); i++)
	{
		outputList.append(QString(OutputVect[i].c_str()));
	}
	QStringList * outputStringList = new QStringList(outputList);
	ui->outputBox->addItems(*outputStringList);

	for(unsigned int i = 0; i < InputVect.size(); i++)
	{
		inputList.append(QString(InputVect[i].c_str()));
	}
	QStringList * inputStringList = new QStringList(inputList);
	ui->inputBox->addItems(*inputStringList);

}

void AudioConfiguration::sendOutputVect(std::vector<std::string> &outputVect )
{
	int numDevices = Pa_GetDeviceCount();
	const PaDeviceInfo *deviceInfo;
	int i;
	for( i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		if (deviceInfo->maxOutputChannels > 0)
			outputVect.push_back(deviceInfo->name);
	}
}

void AudioConfiguration::sendInputVect(std::vector<std::string> &inputVect )
{
	int numDevices = Pa_GetDeviceCount();
	const PaDeviceInfo *deviceInfo;
	int i;
	for( i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		if (deviceInfo->maxInputChannels > 0)
			inputVect.push_back(deviceInfo->name);
	}
}


AudioConfiguration::~AudioConfiguration()
{
	delete ui;
}

void AudioConfiguration::swapShowDialogModeless()
{
	// Si elle est deja affichee, on cache la boite de dialogue sinon on l'affiche
	setVisible(!isVisible());
}

void AudioConfiguration::setInput(QString input)
{
	m_tmpInput = input.mid(0);
}
void AudioConfiguration::setOutput(QString output)
{
	m_tmpOutput = output.mid(0);
}

void AudioConfiguration::acceptData()
{
	m_conf->setAudioInput(m_tmpInput);
	m_conf->setAudioOutput(m_tmpOutput);

	m_conf->setInputIndex(ui->inputBox->currentIndex());
	m_conf->setOutputIndex(ui->outputBox->currentIndex());

	((Controler*) m_parent)->restartEngine();
}
