/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"
#include "Track/TrackLoader.h"
#include <QDebug>
/**
 * @brief GridEditor::GridEditor
 *
 * Constructeur de la grille d'accords.
 */
GridEditor::GridEditor()
{
	newGridDialog = 0;
	trackProperties = new TrackProperties(this);
	audioWindow = new AudioWindow(this);

    setWindowTitle("GridEditor");
    resize(800, 600); //Taille de la fenêtre
    createMenu();
    createActions();
    setActionsToMenu();
    createToolbar();

	createCentralWidget();
    setCentralWidget(centralArea);
    connectActionToSlot();



    connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
    connect(this, SIGNAL(play(int)), audioWindow, SLOT(playFrom(int)));


    settings = new QSettings("GuitarTutor", "GridEditor"); //Permet de retenir la configuration du logiciel

    //Demande au client de choisir entre les deux types d'édition
    if(!settings->contains("ShowEditionSelector") || settings->value("ShowEditionSelector").toBool() == true) {
        editionSelector = new EditionSelector(this);
        editionSelector->setWindowModality(Qt::ApplicationModal);
        editionSelector->show();
        connect(editionSelector, SIGNAL(newEditor(int)), this, SLOT(newEditor(int)));
    }

}

/**
 * @brief GridEditor::~GridEditor
 *
 * Destructeur de la grille d'accords.
 */
GridEditor::~GridEditor() {
    delete grid;
	delete audioWindow;
    delete chordTree;
    delete layout;
    delete fileMenu; delete editMenu; delete optionMenu; delete aboutMenu;
    delete toolBar;
    delete openAction; delete saveAction; delete addRowAction; delete copyDownAction; delete deleteRowAction;
		delete quitAction; delete aboutAction; delete newAction; delete renameAction; delete openAudioWindowAction;
		delete openTrackPropertiesAction; //peut être faire un dictionnaire d'actions qu'on puisse appeler par leur nom.
		//ex. : actions["new"];
    delete settings;
    delete centralArea;
}

//---------------------------------------------------
//-----------------------Début création de la fenêtre
//---------------------------------------------------

/**
 * @brief GridEditor::createMenu
 *
 * Crée le menu principal.
 */
void GridEditor::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu  = menuBar()->addMenu(tr("&Edit"));
    optionMenu = menuBar()->addMenu(tr("&Options"));
    aboutMenu = menuBar()->addMenu(tr("&About"));
}

/**
 * @brief GridEditor::createActions
 *
 * Crée les actions qui serviront dans le menu et dans la barre d'outils.
 */
void GridEditor::createActions(){
    quitAction = new QAction(tr("&Quit"), this);
    aboutAction = new QAction(tr("About"), this);
    newAction = new QAction(tr("&New"), this);
    saveAction = new QAction(tr("&Save"), this);
    openAction = new QAction(tr("&Open"), this);
    addRowAction = new QAction(tr("Add row"), this);
    deleteRowAction = new QAction(tr("Delete row"), this);
    addColumnAction = new QAction(tr("Add column"), this);
    deleteColumnAction = new QAction(tr("Delete column"), this);
    copyDownAction = new QAction(tr("&Copy down"), this);
    renameAction = new QAction(tr("Rename"), this);
	openAudioWindowAction = new QAction(tr("Audio"), this);
	openTrackPropertiesAction = new QAction(tr("Properties"), this);

    quitAction->setIcon(QIcon("icons/quit.png"));
    aboutAction->setIcon(QIcon("icons/about.png"));
    newAction->setIcon(QIcon("icons/new.png"));
    saveAction->setIcon(QIcon("icons/save.png"));
    openAction->setIcon(QIcon("icons/open.png"));
    addRowAction->setIcon(QIcon("icons/addrow.png"));
    deleteRowAction->setIcon(QIcon("icons/deleterow.png"));
    addColumnAction->setIcon(QIcon("icons/addrow.png"));
    deleteColumnAction->setIcon(QIcon("icons/deleterow.png"));

    saveAction->setEnabled(false);
    /*
    deleteRowAction->setEnabled(false);
    copyDownAction->setEnabled(false);
    addRowAction->setEnabled(false);
    */
    renameAction->setEnabled(false);
}

/**
 * @brief GridEditor::setActionsToMenu
 *
 * Ajoute les actions au menu.
 */
void GridEditor::setActionsToMenu() {
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
	fileMenu->addAction(openAudioWindowAction);
    fileMenu->addAction(quitAction);
    editMenu->addAction(addRowAction);
    editMenu->addAction(deleteRowAction);
    editMenu->addAction(addColumnAction);
    editMenu->addAction(deleteColumnAction);
    editMenu->addAction(copyDownAction);
	optionMenu->addAction(openTrackPropertiesAction);
    aboutMenu->addAction(aboutAction);
}

/**
 * @brief GridEditor::createToolbar
 *
 * Crée la barre d'outil principale et y ajoute les actions.
 */
void GridEditor::createToolbar() {
    toolBar = new QToolBar(tr("Tool bar"));
    addToolBar(Qt::RightToolBarArea, toolBar);
    toolBar->addAction(newAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(addRowAction);
    toolBar->addAction(deleteRowAction);
    toolBar->addAction(addColumnAction);
    toolBar->addAction(deleteColumnAction);
}

/**
 * @brief GridEditor::createCentralWidget
 *
 * Création du widget principale de la fenêtre.
 */
void GridEditor::createCentralWidget() {
    centralArea = new QWidget();

    /*Mise en place du layout*/
    chordTree = new ChordTree(); //Initialisation de chord_tree
    grid = new ChordTableWidget(); //Fenere d'accords

    layout = new QGridLayout();

    layout->addWidget(chordTree, 0, 0); //Liste des accords en haut-gauche
	layout->addWidget(grid, 0, 1); //Fenêtre d'accords en haut-milieu
    centralArea->setLayout(layout);
}

/**
 * @brief GridEditor::connectActionToSlot
 *
 * Défini les relations entre signaux et slots pour la fenêtre principale.
 */
void GridEditor::connectActionToSlot()
{


    connect(chordTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));

    connect(addRowAction, SIGNAL(triggered()), grid, SLOT(insert_row()));
    connect(addColumnAction, SIGNAL(triggered()), grid, SLOT(insert_column()));
    connect(deleteColumnAction, SIGNAL(triggered()), grid, SLOT(delete_selected_column()));
    connect(copyDownAction, SIGNAL(triggered()), grid, SLOT(copy_down_rows()));
    connect(deleteRowAction, SIGNAL(triggered()), grid, SLOT(delete_selected_row()));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newGrid()));
    connect(renameAction, SIGNAL(triggered()), this, SLOT(rename()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));


	connect(saveAction, SIGNAL(triggered()), this, SLOT(toXML()));
	connect(openAction, SIGNAL(triggered()), this, SLOT(fromXML()));

	connect(openAudioWindowAction, SIGNAL(triggered()), audioWindow, SLOT(show()));
	connect(openTrackPropertiesAction, SIGNAL(triggered()), trackProperties, SLOT(exec()));
}

//---------------------------------------------------
//-------------------------Fin création de la fenêtre
//---------------------------------------------------

/**
 * @brief GridEditor::changeState
 *
 * Gère l'activation des différents outils dans la fenêtre selon les évènements.
 */
void GridEditor::changeState() {
    if (grid->is_selection_empty() && chordTree->isEnabled())
        chordTree->setEnabled(false);
    else if(!grid->is_selection_empty() && !chordTree->isEnabled())
        chordTree->setEnabled(true);

    /*
    if (grid->is_row_selected() && !deleteRowAction->isEnabled()) {
        deleteRowAction->setEnabled(true);
        copyDownAction->setEnabled(true);
    }
    else if (!grid->is_row_selected() && deleteRowAction->isEnabled()) {
        deleteRowAction->setEnabled(false);
        copyDownAction->setEnabled(false);
    }
    */
}

/**
 * @brief GridEditor::newGrid
 *
 * Mise en place d'une nouvelle grille dans le widget central.
 */
void GridEditor::newGrid()
{
	if (newGridDialog != 0)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("The document has been modified."));
		msgBox.setInformativeText(tr("Do you want to save your changes?"));
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);

		switch (msgBox.exec())
		{
			case QMessageBox::Save:
				saveAction->activate(QAction::Trigger);
				break;
			case QMessageBox::Discard:
				delete newGridDialog;
				break;
			case QMessageBox::Cancel:
				return;
				break;
			default:
				return;
				break;
		}
	}

	newGridDialog = new NewGridDialog(this);
	int accept = newGridDialog->exec();
	if(accept == QDialog::Rejected)
	{
		return;
	}
	else
	{
		delete grid;
		grid = new ChordTableWidget(newGridDialog->getColumns() + 1, newGridDialog->getLines(), this);
		trackProperties->setTrack(newGridDialog->getTrack());
		trackProperties->setArtist(newGridDialog->getArtist());
		trackProperties->setBarSize(newGridDialog->getBarSize());

		layout->addWidget(grid, 0, 1);
		saveAction->setEnabled(true);
		addRowAction->setEnabled(true);
		addColumnAction->setEnabled(true);
		renameAction->setEnabled(true);

		connect(chordTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
		connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
		connect(addRowAction, SIGNAL(triggered()), grid, SLOT(insert_row()));
		connect(copyDownAction, SIGNAL(triggered()), grid, SLOT(copy_down_rows()));
		connect(deleteRowAction, SIGNAL(triggered()), grid, SLOT(delete_selected_row()));
		connect(deleteColumnAction, SIGNAL(triggered()), grid, SLOT(delete_selected_column()));
		connect(addColumnAction, SIGNAL(triggered()), grid, SLOT(insert_column()));

		connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
	}
}


/**
 * @brief GridEditor::newEditor
 * @param type Type d'éditeur à créer
 *
 * Demande la création de l'éditeur demandé (utilisé depuis l'assistant).
 */
void GridEditor::newEditor(int type)
{
    if(type == ASSISTED_EDITOR)
        QMessageBox::information(this, tr("So sorry..."), tr("This function has not been implemented yet, but come back soon!"));
    else if(type == MANUAL_EDITOR)
        newGrid();
    delete editionSelector;
}

/**
 * @brief GridEditor::toXML
 *
 * Slot pour la sauvegarde d'un fichier XML.
 */
// penser à faire en sorte que les propriétés de base du morceau soient obligatoires (nom, etc...)
void GridEditor::toXML() //ça serait bien qu'on sélectionne le fichier ou on sauve.
{
	// Pour tester qu'on sauve bien un xml
	int pos = 0;
	QRegExp xmlExt("\\S+\\.xml");
	QRegExpValidator *validator = new QRegExpValidator(xmlExt, this);

	QString file = QFileDialog::getSaveFileName(this, "Sauvegarde", ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);
	if(validator->validate(file, pos) != QValidator::Acceptable)
	{
		file.append(".xml");
	}

	LogicalTrack* track = grid->getLogicalTrack();

	track->setTrackName(trackProperties->getTrack());
	track->setArtist(trackProperties->getArtist());
	track->setMesure(trackProperties->getBarSize());

	track->setAudioFileName(audioWindow->getFilename()); //vérifier si chemin absolu

	TrackLoader::convertLogicalTrackToXml(track, file);
}

/**
 * @brief GridEditor::fromXML
 *
 * Slot pour le chargement d'un fichier XML.
 */
void GridEditor::fromXML() //ça serait bien qu'on sélectionne le fichier ou on sauve.
{
	LogicalTrack* track = new LogicalTrack;
	TrackLoader::convertXmlToLogicalTrack("test.xml", track); //tester la valeur de retour et afficher dialog si échec

	trackProperties->setTrack(track->getTrackName());
	trackProperties->setArtist(track->getArtist());

	audioWindow->setAudioFileName(track->getAudioFileName()); //vérifier si chemin absolu
	audioWindow->setAudioFile();

	// il faudra penser à recalculer le début, la fin et la durée de la première mesure pour les mettre
	// dans audiowindow

	grid->setLogicalTrack(track);
}
