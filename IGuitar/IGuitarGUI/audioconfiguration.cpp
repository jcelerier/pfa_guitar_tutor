#include "audioconfiguration.h"
#include "ui_audioconfiguration.h"

#include <string>
#include <QString>
#include "../IGuitar/src/MusicManager.h"
#include "../IGuitar/src/ScoreManager.h"
#include <iostream>

AudioConfiguration::AudioConfiguration(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AudioConfiguration)
{
	ui->setupUi(this);
}

void AudioConfiguration::init(void)
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
	tmpInput = input.mid(0);
}
void AudioConfiguration::setOutput(QString output)
{
	tmpOutput = output.mid(0);
}
void AudioConfiguration::acceptData()
{
	chosenInput = tmpInput.mid(0);
	chosenOutput = tmpOutput.mid(0);
}
