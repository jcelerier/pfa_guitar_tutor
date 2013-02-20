#include "AudioSync.h"
#include "Util.hpp"
#include <QMessageBox>
#include <QDebug>




AudioSync::AudioSync()
{
    layout = new QGridLayout();
	beginning = new TimeEdit();
	end = new TimeEdit();
	bar = new TimeEdit();
	bpm = new QSpinBox();


    lbeginning = new QLabel(tr("Beginning"));
    lend = new QLabel(tr("End"));
    lbar = new QLabel(tr("Bar"));
	lbpm = new QLabel(tr("BPM"));

    bbeginning = new QToolButton();
    bend = new QToolButton();
    bbar = new QToolButton();

    sendButton = new QPushButton(tr("Compute"));

    bbeginning->setIcon(QIcon("icons/timer.png"));
    bend->setIcon(QIcon("icons/timer.png"));
    bbar->setIcon(QIcon("icons/timer.png"));

	beginning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    activeButtons(false);

	bpm->setMinimum(10);
	bpm->setMaximum(250);
	beginning->setDisplayFormat("m:ss:zzz");
	end->setDisplayFormat("m:ss:zzz");
	end->setBad();
	bar->setDisplayFormat("m:ss:zzz");
	bar->setBad();

	layout->addWidget(lbeginning, 0, 0);
	layout->addWidget(beginning, 0, 1);
    layout->addWidget(bbeginning, 0, 2);

	layout->addWidget(lbar, 1, 0);
	layout->addWidget(bar, 1, 1);
	layout->addWidget(bbar, 1, 2);

	layout->addWidget(lend, 2, 0);
	layout->addWidget(end, 2, 1);
	layout->addWidget(bend, 2, 2);


	layout->addWidget(sendButton, 3, 0);

	layout->addWidget(lbpm, 4, 0);
	layout->addWidget(bpm, 4, 1);

    setLayout(layout);

    connect(bbeginning, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bend, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bbar, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));

	connect(beginning, SIGNAL(timeChanged(QTime)), this, SLOT(beginningChanged(QTime)));
	connect(bar, SIGNAL(timeChanged(QTime)), this, SLOT(barChanged(QTime)));
	connect(end, SIGNAL(timeChanged(QTime)), this, SLOT(endChanged(QTime)));

	connect(bpm, SIGNAL(valueChanged(int)), this, SLOT(tempoChanged(int)));

	connect(sendButton, SIGNAL(clicked()), this, SLOT(sendData()));
}

AudioSync::~AudioSync()
{

	delete bpm;
	delete beginning;
    delete end;
    delete bar;

	delete layout;

    delete bbeginning;
    delete bend;
    delete bbar;

	delete lbpm;
    delete lend;
    delete lbeginning;
    delete lbar;


}

void AudioSync::activeButtons(bool active)
{
	beginning->setEnabled(active);
    end->setEnabled(active);
    bar->setEnabled(active);

	if(active)
	{
		endChanged(end->time());
		beginningChanged(beginning->time());
		barChanged(bar->time());
	}
}

void AudioSync::setBegginingTimer(const QTime t)
{
	beginning->setTime(t);
}

void AudioSync::setEndTimer(const QTime t)
{
    end->setTime(t);
}

void AudioSync::setBarTimer(const QTime t)
{
    bar->setTime(t);
}

void AudioSync::beginningChanged(QTime t)
{
	checkTimes();
	emit sendTimer(TIMER_BEGINNING, t);
}

void AudioSync::barChanged(QTime t)
{
	checkTimes();
	int tps = TimeToMsec(t) - TimeToMsec(beginning->time());
	if(tps > 0)
	{
		int tempo = 240000 / tps ;

		if(tempo > 10 && tempo < 250 && tempo != bpm->value())
			bpm->setValue(tempo);
	}


	emit sendTimer(TIMER_BAR, t);
}

void AudioSync::endChanged(QTime t)
{
	checkTimes();
	emit sendTimer(TIMER_END, t);
}

/**
 * @brief AudioSync::checkTimes
 *
 * On doit avoir à tout instant beginning < bar < end
 */
void AudioSync::checkTimes()
{
	long long tps = TimeToMsec(end->time()) - TimeToMsec(beginning->time());
	if(tps > 0)
	{
		end->setGood();
	}
	else
	{
		end->setBad();
	}

	long long tps1 = TimeToMsec(end->time()) - TimeToMsec(bar->time());
	long long tps2 = TimeToMsec(bar->time()) - TimeToMsec(beginning->time());
	if(tps1 > 0 && tps2 > 0)
	{
		bar->setGood();
	}
	else
	{
		bar->setBad();
	}
}


void AudioSync::tempoChanged(int tempo)
{
	QTime bar_time = MsecToTime(240000 / tempo + TimeToMsec(beginning->time()));

	bar->setTime(bar_time);
}

void AudioSync::emitSignalTimer()
{
    if(bbeginning->isDown())
		emit refreshTimer(TIMER_BEGINNING);
    else if(bend->isDown())
        emit refreshTimer(TIMER_END);
    else if(bbar->isDown())
        emit refreshTimer(TIMER_BAR);
}

void AudioSync::sendData()
{
    if(beginning->time().isValid() && bar->time().isValid() && end->time().isValid())
	{
		emit sendTimers(beginning->time(), bar->time(), end->time());
	}
    else
		QMessageBox::information(this, tr("Your job is not done yet"), tr("You have not set the three timers yet."));
}
