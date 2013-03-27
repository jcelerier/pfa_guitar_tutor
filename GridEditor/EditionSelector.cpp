#include "EditionSelector.h"
#include "GridEditor.h"

/**
 * @brief EditionSelector::EditionSelector
 * @param parent Fenêtre parente
 *
 * Crée la fenêtre d'invite de sélection d'éditeur.
 */
EditionSelector::EditionSelector(QWidget *parent) :
	QWidget(parent)
{
	setWindowFlags(Qt::Dialog);
	m_presentation = new QLabel(tr("What do you want to do today?"));
	m_tabs = new QTabWidget(this);
	m_mainLayout = new QVBoxLayout();

	m_pageNew = new QWidget(); //Pages pour les onglets
	m_pageOpen = new QWidget();

	m_vboxNew = new QVBoxLayout;
	m_vboxOpen = new QVBoxLayout;

	//Page nouveau fichier
	NewGridDialog* newGridDialog = new NewGridDialog();
	((GridEditor*) parent)->m_newGridDialog = newGridDialog;

	//Page ouverture fichier
	QLabel *pictureOpen = new QLabel(tr("Press Load to load a file"));
	pictureOpen->setWordWrap(true);
	QPushButton *buttonOpen = new QPushButton(tr("Load"));


	m_vboxNew->addWidget(newGridDialog);
	m_pageNew->setLayout(m_vboxNew);

	m_vboxOpen->addWidget(pictureOpen);
	m_vboxOpen->addWidget(buttonOpen);
	m_pageOpen->setLayout(m_vboxOpen);

	m_tabs->addTab(m_pageNew, tr("New grid"));
	m_tabs->addTab(m_pageOpen, tr("Open a grid"));

	m_mainLayout->addWidget(m_presentation);
	m_mainLayout->addWidget(m_tabs);
	setLayout(m_mainLayout);

	connect(newGridDialog, SIGNAL(accepted()), parent, SLOT(firstNewGrid()));
	connect(buttonOpen, SIGNAL(clicked()), parent, SLOT(fromXML()));
}

/**
 * @brief EditionSelector::~EditionSelector
 *
 * Destructeur.
 */
EditionSelector::~EditionSelector()
{
	delete m_presentation;
	delete m_tabs;
	delete m_mainLayout;

	//les autres membres sont delete par ceux-là
}
