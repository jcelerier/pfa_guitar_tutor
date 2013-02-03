#include "EditionSelector.h"
#include "GridEditor.h"

#include <QDebug>
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
	presentation = new QLabel(tr("What do you want to do today?"));
	tabs = new QTabWidget(this);
	mainLayout = new QVBoxLayout();

	pageNew = new QWidget(); //Pages pour les onglets
	pageOpen = new QWidget();

	vboxNew = new QVBoxLayout;
	vboxOpen = new QVBoxLayout;

	//Page nouveau fichier
	NewGridDialog* newGridDialog = new NewGridDialog();
	((GridEditor*) parent)->newGridDialog = newGridDialog;

	//Page ouverture fichier
	QLabel *pictureOpen = new QLabel(tr("Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi."));
	pictureOpen->setWordWrap(true);
	QPushButton *buttonOpen = new QPushButton(tr("Begin"));


	vboxNew->addWidget(newGridDialog);
	pageNew->setLayout(vboxNew);

	vboxOpen->addWidget(pictureOpen);
	vboxOpen->addWidget(buttonOpen);
	pageOpen->setLayout(vboxOpen);

	tabs->addTab(pageNew, tr("New grid"));
	tabs->addTab(pageOpen, tr("Open a grid"));

	mainLayout->addWidget(presentation);
	mainLayout->addWidget(tabs);
	setLayout(mainLayout);

	connect(newGridDialog, SIGNAL(accepted()), parent, SLOT(firstNewGrid()));
	connect(buttonOpen, SIGNAL(clicked()), parent, SLOT(fromXML()));
}


EditionSelector::~EditionSelector()
{
	qDebug() << "here";
	delete presentation;
	delete tabs;
	delete mainLayout;

	//les autres membres sont delete par ceux-là
}
