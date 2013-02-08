#include "EditorPanel.h"
#include <QDebug>

/**
 * @brief EditorPanel::EditorPanel
 * @param chordTable Pointeur vers la grille de GridEditor
 * @param audioWindow Pointeur vers l'audioWindow de GridEditor
 * @param parent Parent Qt
 */
EditorPanel::EditorPanel(ChordTableWidget* chordTable, AudioWindow* audioWindow, QWidget *parent) :
	QWidget(parent)
{

	setWindowFlags(Qt::Dialog);
    tabs = new QTabWidget(this);
    mainLayout = new QVBoxLayout();

    pageChordTable = new QWidget(); //Pages pour les onglets
    pageAudio = new QWidget();

	//Page nouveau fichier

    vboxTable = new QVBoxLayout();
	vboxTable->addWidget(chordTable);

	pageChordTable->setLayout(vboxTable);

	//Page ouverture fichier

    vboxAudio = new QVBoxLayout;
	vboxAudio->addWidget(audioWindow);

	pageAudio->setLayout(vboxAudio);
    spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vboxAudio->addItem( spacer );

	tabs->addTab(pageChordTable, tr("Grid"));
	tabs->addTab(pageAudio, tr("Audio edition"));

	mainLayout->addWidget(tabs);
	setLayout(mainLayout);
}
