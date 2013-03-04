#include "audioconfiguration.h"
#include "ui_audioconfiguration.h"

#include <string>
#include <QString>
#include <MusicManager.h>
#include <ScoreManager.h>
#include <iostream>
#include <algorithm>
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
	fillInputVect(InputVect);
	fillOutputVect(OutputVect);

	PaDeviceIndex inputDefault = Pa_GetDefaultInputDevice();
	PaDeviceIndex outputDefault = Pa_GetDefaultOutputDevice();
	Pa_Terminate();

	// on met la valeur par défaut... eh bien... par défaut.
	m_conf->setInputIndex(inputDefault);
	m_conf->setOutputIndex(outputDefault);

	std::vector<int>::iterator i_iter = std::find(inputIndexes.begin(), inputIndexes.end(), inputDefault);
	int i_index = i_iter - inputIndexes.begin();

	std::vector<int>::iterator o_iter = std::find(outputIndexes.begin(), outputIndexes.end(), outputDefault);
	int o_index = o_iter - outputIndexes.begin();

	for(unsigned int i = 0; i < OutputVect.size(); i++)
	{
		outputList.append(QString(OutputVect[i].c_str()));
	}
	QStringList * outputStringList = new QStringList(outputList);
	ui->outputBox->addItems(*outputStringList);
	ui->outputBox->setCurrentIndex(o_index);

	for(unsigned int i = 0; i < InputVect.size(); i++)
	{
		inputList.append(QString(InputVect[i].c_str()));
	}
	QStringList * inputStringList = new QStringList(inputList);
	ui->inputBox->addItems(*inputStringList);
	ui->inputBox->setCurrentIndex(i_index);


}

void AudioConfiguration::fillOutputVect(std::vector<std::string> &outputVect )
{
	int numDevices = Pa_GetDeviceCount();
	const PaDeviceInfo *deviceInfo;
	int i;
	for( i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		if (deviceInfo->maxOutputChannels > 0)
		{
			outputVect.push_back(deviceInfo->name);
			outputIndexes.push_back(i);
		}
	}
}

void AudioConfiguration::fillInputVect(std::vector<std::string> &inputVect )
{
	int numDevices = Pa_GetDeviceCount();
	const PaDeviceInfo *deviceInfo;
	int i;
	for( i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		if (deviceInfo->maxInputChannels > 0)
		{
			inputVect.push_back(deviceInfo->name);
			inputIndexes.push_back(i);
		}
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

	qDebug() << "AudioConfiguration::acceptData" << ui->inputBox->currentIndex();
	for(int i = 0; i < inputIndexes.size(); i ++)
	{
		qDebug() << i << inputIndexes[i] << Pa_GetDeviceInfo(inputIndexes[i])->name;
	}
	m_conf->setInputIndex(inputIndexes[ui->inputBox->currentIndex()]);
	m_conf->setOutputIndex(outputIndexes[ui->outputBox->currentIndex()]);

	((Controler*) m_parent)->restartEngine();
}
