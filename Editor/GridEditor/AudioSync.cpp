#include "AudioSync.h"
#include <QMessageBox>
#include <QDebug>
AudioSync::AudioSync()
{
    layout = new QGridLayout();
	beginning = new QTimeEdit();
    end = new QTimeEdit();
    bar = new QTimeEdit();

	beginning->setDisplayFormat("m:ss:zzz");
    end->setDisplayFormat("m:ss:zzz");
    bar->setDisplayFormat("m:ss:zzz");

    lbeginning = new QLabel(tr("Beginning"));
    lend = new QLabel(tr("End"));
    lbar = new QLabel(tr("Bar"));
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
    setLayout(layout);

    connect(bbeginning, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bend, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bbar, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));

	connect(beginning, SIGNAL(timeChanged(QTime)), this, SLOT(beginningChanged(QTime)));
	connect(bar, SIGNAL(timeChanged(QTime)), this, SLOT(barChanged(QTime)));
	connect(end, SIGNAL(timeChanged(QTime)), this, SLOT(endChanged(QTime)));

	connect(sendButton, SIGNAL(clicked()), this, SLOT(sendData()));
}

AudioSync::~AudioSync()
{
	delete beginning;
    delete end;
    delete bar;
    delete layout;
    delete bbeginning;
    delete bend;
    delete bbar;
    delete lend;
    delete lbeginning;
    delete lbar;
}

void AudioSync::activeButtons(bool active)
{
	beginning->setEnabled(active);
    end->setEnabled(active);
    bar->setEnabled(active);
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
	emit sendTimer(TIMER_BEGINNING, t);
}

void AudioSync::barChanged(QTime t)
{
	emit sendTimer(TIMER_BAR, t);
}

void AudioSync::endChanged(QTime t)
{
	emit sendTimer(TIMER_END, t);
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
