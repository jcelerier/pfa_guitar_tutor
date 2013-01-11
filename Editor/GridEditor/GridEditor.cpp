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
GridEditor::GridEditor() {
    setWindowTitle("GridEditor");
    resize(800, 600); //Taille de la fenêtre
    createMenu();
    createActions();
    setActionsToMenu();
    createToolbar();

    createCentralWidget();
    setCentralWidget(centralArea);
    connectActionToSlot();

    //Demande au client de choisir entre les deux types d'édition
    editionSelector = new EditionSelector(this);
    editionSelector->setWindowModality(Qt::ApplicationModal);
    editionSelector->show();

	trackProperties = new TrackProperties(this);
    //Redirection vers la fonction affichant l'éditeur sélectionné
    connect(editionSelector, SIGNAL(newEditor(int)), this, SLOT(newEditor(int)));

	connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
	connect(this, SIGNAL(play(int)), audioWindow, SLOT(playFrom(int)));
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
        delete quitAction; delete aboutAction; delete newAction; delete renameAction;

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

	audioWindow = new AudioWindow(this);

    layout->addWidget(chordTree, 0, 0); //Liste des accords en haut-gauche
	layout->addWidget(grid, 0, 1); //Fenêtre d'accords en haut-milieu
    centralArea->setLayout(layout);
}

/**
 * @brief GridEditor::connectActionToSlot
 *
 * Défini les relations entre signaux et slots pour la fenêtre principale.
 */
void GridEditor::connectActionToSlot(){
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

	connect(openAudioWindowAction, SIGNAL(triggered()), this, SLOT(openAudioWindow()));
	connect(openTrackPropertiesAction, SIGNAL(triggered()), this, SLOT(openTrackProperties()));
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
// A FACTORISER DE TOUTE URGENCE AVEC LE CODE DU CONSTRUCTEUR !!
void GridEditor::newGrid() {
    bool ok;
    QString mess;
    if (grid->rowCount() == 0)
        mess = tr("Column number:");
    else
        mess = tr("Warning:\nUnsaved modifications on current grid will be lost.\n\nColumn number:");
    int column = QInputDialog::getInt(this, tr("New grid"), tr(mess.toAscii()), 4, 1, 64, 1, &ok);
    if (!ok)
        return;
    delete grid;
	grid = new ChordTableWidget(column + 1, 10, this);
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


void GridEditor::openAudioWindow()
{
	audioWindow->show();
}

/*
*/
#include <QDebug>
// penser à faire en sorte que les propriétés de base du morceau soient obligatoires (nom, etc...)
void GridEditor::toXML() //ça serait bien qu'on sélectionne le fichier ou on sauve.
{
	LogicalTrack* track = grid->getLogicalTrack();

	track->setTrackName(trackProperties->getTrack());
	track->setArtist(trackProperties->getArtist());
	track->setMesure(trackProperties->getBarSize());

	track->setAudioFileName(audioWindow->getSong()); //vérifier si chemin absolu

	TrackLoader::convertLogicalTrackToXml(track);
}

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

void GridEditor::openTrackProperties()
{
	trackProperties->exec();
}
