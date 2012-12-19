#include "AudioSync.h"

AudioSync::AudioSync()
{
    layout = new QGridLayout();
    beggining = new QTimeEdit();
    end = new QTimeEdit();
    bar = new QTimeEdit();

    beggining->setDisplayFormat("m:ss:zzz");
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

    beggining->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    activeButtons(false);

    layout->addWidget(lbeggining, 0, 0);
    layout->addWidget(beggining, 0, 1);
    layout->addWidget(bbeggining, 0, 2);
    layout->addWidget(lend, 1, 0);
    layout->addWidget(end, 1, 1);
    layout->addWidget(bend, 1, 2);
    layout->addWidget(lbar, 2, 0);
    layout->addWidget(bar, 2, 1);
    layout->addWidget(bbar, 2, 2);

    setLayout(layout);
}

AudioSync::~AudioSync()
{
    delete beggining;
    delete end;
    delete bar;
    delete layout;
    delete bbeggining;
    delete bend;
    delete bbar;
}

void AudioSync::activeButtons(bool active)
{
    beggining->setEnabled(active);
    end->setEnabled(active);
    bar->setEnabled(active);
}
