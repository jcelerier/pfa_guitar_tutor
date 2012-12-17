#include "AudioSync.h"

AudioSync::AudioSync()
{
    QGridLayout* layout = new QGridLayout();
    beggining = new QLineEdit();
    end = new QLineEdit();
    bar = new QLineEdit();

    layout->addWidget(beggining, 0, 1);
    layout->addWidget(end, 1, 1);
    layout->addWidget(bar, 2, 1);

    setLayout(layout);

    delete layout;
}

AudioSync::~AudioSync()
{
    delete beggining;
    delete end;
    delete bar;
}
