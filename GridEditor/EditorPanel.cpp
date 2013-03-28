#include "EditorPanel.h"


/**
 * @brief EditorPanel::EditorPanel
 * @param chordTable Pointeur vers la grille de GridEditor
 * @param audioWindow Pointeur vers l'audioWindow de GridEditor
 * @param trackProperties Pointeur vers le trackProperties de GridEditor
 * @param parent Parent Qt
 *
 * Cette classe sert à afficher les onglets dans la fenêtre principale.
 * Attention : les données des onglets sont possédées par le parent,
 * EditorPanel n'est qu'une sorte de vue.
 */
EditorPanel::EditorPanel(ChordTableWidget* chordTable,
						 AudioWindow* audioWindow,
						 TrackProperties* trackProperties,
						 QWidget *parent) :
	QWidget(parent)
{

	setWindowFlags(Qt::Dialog);
	m_tabs = new QTabWidget(this);
	m_mainLayout = new QVBoxLayout();

	m_pageChordTable = new QWidget(); //Pages pour les onglets
	m_pageAudio = new QWidget();
	m_pageProps = new QWidget();

	//Page grille
	m_vboxTable = new QVBoxLayout();
	if(chordTable != 0)
	{
		m_vboxTable->addWidget(chordTable);
	}

	m_grid = chordTable;

	m_pageChordTable->setLayout(m_vboxTable);

	//Page audio
	m_vboxAudio = new QVBoxLayout;
	m_spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );

	m_vboxAudio->addWidget(audioWindow);
	m_vboxAudio->addItem( m_spacer );

	m_pageAudio->setLayout(m_vboxAudio);


	//Page propriétés
	m_vboxProps = new QVBoxLayout;
	m_vboxProps->addWidget(trackProperties);

	m_pageProps->setLayout(m_vboxProps);
	//-- Général --//

	m_tabs->addTab(m_pageChordTable, tr("Grid"));
	m_tabs->addTab(m_pageAudio, tr("Audio edition"));
	m_tabs->addTab(m_pageProps, tr("Properties"));

	m_mainLayout->addWidget(m_tabs);
	setLayout(m_mainLayout);
}


/**
 * @brief EditorPanel::updateGrid
 * @param chordTable La nouvelle grille
 *
 * Changement de grille lors d'un chargement ou d'un nouveau fichier
 */
void EditorPanel::updateGrid(ChordTableWidget *chordTable)
{
	if(m_grid != 0)
	{
		m_vboxTable->removeWidget(m_grid);
	}
	m_vboxTable->addWidget(chordTable);
	m_grid = chordTable;
}

/**
 * @brief EditorPanel::~EditorPanel
 *
 * Destructeur.
 */
EditorPanel::~EditorPanel() {
	delete m_mainLayout;
	delete m_tabs;
}
