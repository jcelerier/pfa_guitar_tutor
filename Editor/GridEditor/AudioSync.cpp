#include "AudioSync.h"

AudioSync::AudioSync()
{
    layout = new QGridLayout();
	beginning = new QTimeEdit();
    end = new QTimeEdit();
    bar = new QTimeEdit();

	beginning->setDisplayFormat("m:ss:zzz");
    end->setDisplayFormat("m:ss:zzz");
    bar->setDisplayFormat("m:ss:zzz");

    QLabel* lbeggining = new QLabel(tr("Beggining"));
    QLabel* lend = new QLabel(tr("End"));
    QLabel* lbar = new QLabel(tr("Bar"));
    bbeggining = new QToolButton();
    bend = new QToolButton();
    bbar = new QToolButton();

    bbeggining->setIcon(QIcon("icons/timer.png"));
    bend->setIcon(QIcon("icons/timer.png"));
    bbar->setIcon(QIcon("icons/timer.png"));

	beginning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    activeButtons(false);

    layout->addWidget(lbeggining, 0, 0);
	layout->addWidget(beginning, 0, 1);
    layout->addWidget(bbeggining, 0, 2);
    layout->addWidget(lend, 1, 0);
    layout->addWidget(end, 1, 1);
    layout->addWidget(bend, 1, 2);
    layout->addWidget(lbar, 2, 0);
    layout->addWidget(bar, 2, 1);
    layout->addWidget(bbar, 2, 2);

    setLayout(layout);

    connect(bbeggining, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bend, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bbar, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
}

AudioSync::~AudioSync()
{
	delete beginning;
    delete end;
    delete bar;
    delete layout;
    delete bbeggining;
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
    if(bbeggining->isDown())
        emit refreshTimer(TIMER_BEGGINING);
    else if(bend->isDown())
        emit refreshTimer(TIMER_END);
    else if(bbar->isDown())
        emit refreshTimer(TIMER_BAR);
}
