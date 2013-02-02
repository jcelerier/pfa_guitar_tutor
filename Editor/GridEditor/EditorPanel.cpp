#include "EditorPanel.h"

EditorPanel::EditorPanel(ChordTableWidget* chordTable, AudioWindow* audioWindow, QWidget *parent) :
	QWidget(parent)
{
	setWindowFlags(Qt::Dialog);
	QTabWidget *tabs = new QTabWidget(this);
	QVBoxLayout *mainLayout = new QVBoxLayout();

	QWidget *pageChordTable = new QWidget(); //Pages pour les onglets
	QWidget *pageAudio = new QWidget();

	//Page nouveau fichier

	QVBoxLayout *vboxTable = new QVBoxLayout;
	vboxTable->addWidget(chordTable);

	pageChordTable->setLayout(vboxTable);


	//Page ouverture fichier

	QVBoxLayout *vboxAudio = new QVBoxLayout;
	vboxAudio->addWidget(audioWindow);

	pageAudio->setLayout(vboxAudio);

	tabs->addTab(pageChordTable, tr("Grid"));
	tabs->addTab(pageAudio, tr("Audio edition"));

	mainLayout->addWidget(tabs);
	setLayout(mainLayout);

}
