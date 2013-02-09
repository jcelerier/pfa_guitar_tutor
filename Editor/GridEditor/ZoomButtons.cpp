#include "ZoomButtons.h"

ZoomButtons::ZoomButtons(QWidget *parent) :
	QWidget(parent)
{
	QGridLayout* layout = new QGridLayout();

	QPushButton *full, *bar, *song;
	full = new QPushButton("100%");
	bar = new QPushButton("Mesure");
	song = new QPushButton("Morceau");

	layout->addWidget(full, 0, 0);
	layout->addWidget(song, 0, 1);
	layout->addWidget(bar, 0, 2);

	setLayout(layout);
}
