#include "EditorPanel.h"
#include <QDebug>

/**
 * @brief EditorPanel::EditorPanel
 * @param chordTable Pointeur vers la grille de GridEditor
 * @param audioWindow Pointeur vers l'audioWindow de GridEditor
 * @param parent Parent Qt
 */
EditorPanel::EditorPanel(ChordTableWidget* chordTable,
						 AudioWindow* audioWindow,
						 TrackProperties* trackProperties,
						 QWidget *parent) :
	QWidget(parent)
{

	setWindowFlags(Qt::Dialog);
    tabs = new QTabWidget(this);
    mainLayout = new QVBoxLayout();

    pageChordTable = new QWidget(); //Pages pour les onglets
    pageAudio = new QWidget();
	pageProps = new QWidget();

	//Page grille
    vboxTable = new QVBoxLayout();
	if(chordTable != 0)
	{
		vboxTable->addWidget(chordTable);
	}

	grid = chordTable;

	pageChordTable->setLayout(vboxTable);

	//Page audio
    vboxAudio = new QVBoxLayout;
	spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );

	vboxAudio->addWidget(audioWindow);
	vboxAudio->addItem( spacer );

	pageAudio->setLayout(vboxAudio);


	//Page propriétés
	vboxProps = new QVBoxLayout;
	vboxProps->addWidget(trackProperties);

	pageProps->setLayout(vboxProps);
	//-- Général --//

	tabs->addTab(pageChordTable, tr("Grid"));
	tabs->addTab(pageAudio, tr("Audio edition"));
    tabs->addTab(pageProps, tr("Properties"));

	mainLayout->addWidget(tabs);
	setLayout(mainLayout);
}

void EditorPanel::updateGrid(ChordTableWidget *chordTable)
{
	if(grid != 0)
	{
		vboxTable->removeWidget(grid);
	}
	vboxTable->addWidget(chordTable);
	grid = chordTable;
}
