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

void AudioConfiguration::init(ScoreManager * scoreManager)
{
	MusicManager *manager = scoreManager->getMusicManager();
	std::vector <std::string> OutputVect;
	manager->sendOutputVect(OutputVect);
	QList <QString> outputList;


	std::vector <std::string> InputVect;
	manager->sendInputVect(InputVect);
	QList <QString> inputList;

	for(unsigned int i = 0; i < OutputVect.size(); i++)
	{
		outputList.append(QString(OutputVect[i].c_str()));
	}
	QStringList * outputStringList = new QStringList(outputList);
	ui->outputBox->addItems(*outputStringList);
/*
	for(int i = 0; i < InputVect.size(); i++)
	{
		ui->inputBox->addItem(InputVect[i]);
	}
*/
}

AudioConfiguration::~AudioConfiguration()
{
	delete ui;
}
