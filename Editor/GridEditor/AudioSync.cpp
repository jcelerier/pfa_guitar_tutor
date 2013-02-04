#include "AudioSync.h"
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

    QLabel* lbeggining = new QLabel(tr("Beginning"));
    QLabel* lend = new QLabel(tr("End"));
    QLabel* lbar = new QLabel(tr("Bar"));
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

    layout->addWidget(lbeggining, 0, 0);
	layout->addWidget(beginning, 0, 1);
    layout->addWidget(bbeginning, 0, 2);
    layout->addWidget(lend, 1, 0);
    layout->addWidget(end, 1, 1);
    layout->addWidget(bend, 1, 2);
    layout->addWidget(lbar, 2, 0);
    layout->addWidget(bar, 2, 1);
    layout->addWidget(bbar, 2, 2);

	layout->addWidget(sendButton, 3, 0);
    setLayout(layout);

    connect(bbeginning, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bend, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bbar, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));

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

void AudioSync::emitSignalTimer()
{
    if(bbeginning->isDown())
        emit refreshTimer(TIMER_BEGGINING);
    else if(bend->isDown())
        emit refreshTimer(TIMER_END);
    else if(bbar->isDown())
        emit refreshTimer(TIMER_BAR);
}

void AudioSync::sendData()
{
    if(beginning->time().isValid() && bar->time().isValid() && end->time().isValid())
	{
        emit sendTimer(beginning->time(), bar->time(), end->time());
	}
    else
        QMessageBox::information(this, tr("Your job is not done yet"), tr("You have not completed the three timers yet."));
}
