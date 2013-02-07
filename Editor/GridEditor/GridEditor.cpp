/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"
#include <QFileDialog>
#include <QDebug>
/**
 * @brief GridEditor::GridEditor
 *
 * Constructeur de la grille d'accords.
 */
GridEditor::GridEditor()
{
    isGridSet = false;
    trackProperties = new TrackProperties(this);
    audioWindow = new AudioWindow(this);
	grid = 0;

	status = statusBar();
	statusInfo = new QLabel(statusText(), status);
	status->addPermanentWidget(statusInfo, 1);
	status->setSizeGripEnabled(false);

    setWindowTitle("GridEditor");
    resize(800, 600); //Taille de la fenêtre
    createMenu();
    createActions();
    setActionsToMenu();
    createToolbar();

	createCentralWidget();
    setCentralWidget(centralArea);
    connectActionToSlot();

    settings = new QSettings("GuitarTutor", "GridEditor"); //Permet de retenir la configuration du logiciel
}

/**
 * @brief GridEditor::~GridEditor
 *
 * Destructeur de la grille d'accords.
 */
GridEditor::~GridEditor() {
    delete openAction; delete saveAction; delete addRowAction; delete copyDownAction; delete deleteRowAction; delete saveAsAction;
		delete quitAction; delete aboutAction; delete newAction; delete renameAction; delete openAudioWindowAction;
        delete openTrackPropertiesAction; delete addColumnAction; delete deleteColumnAction; delete helpAction; //peut être faire un dictionnaire d'actions qu'on puisse appeler par leur nom.
		//ex. : actions["new"];
    delete fileMenu; delete editMenu; delete optionMenu; delete gridMenu; delete aboutMenu;
    delete toolBar;
    delete settings;
    delete trackProperties;
    delete chordTree;
	if(grid != 0)
	{
		delete grid;
	}
	delete editionSelector;
    delete layout;
    delete centralArea;
    delete statusInfo; delete status;
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
    editMenu = menuBar()->addMenu(tr("&Edit"));
    gridMenu = menuBar()->addMenu(tr("&Grid"));
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
    saveAsAction = new QAction(tr("Save as"), this);
    openAction = new QAction(tr("&Open"), this);
    addRowAction = new QAction(tr("Add row"), this);
    deleteRowAction = new QAction(tr("Delete row"), this);
    addColumnAction = new QAction(tr("Add column"), this);
    deleteColumnAction = new QAction(tr("Delete column"), this);
    copyDownAction = new QAction(tr("&Copy down"), this);
    renameAction = new QAction(tr("Rename"), this);
	openAudioWindowAction = new QAction(tr("Audio"), this);
	openTrackPropertiesAction = new QAction(tr("Properties"), this);
    helpAction = new QAction(tr("&Help"), this);

    quitAction->setIcon(QIcon("icons/quit.png"));
    aboutAction->setIcon(QIcon("icons/about.png"));
    newAction->setIcon(QIcon("icons/new.png"));
    saveAction->setIcon(QIcon("icons/save.png"));
    openAction->setIcon(QIcon("icons/open.png"));
    addRowAction->setIcon(QIcon("icons/addrow.png"));
    deleteRowAction->setIcon(QIcon("icons/deleterow.png"));
    addColumnAction->setIcon(QIcon("icons/addrow.png"));
    deleteColumnAction->setIcon(QIcon("icons/deleterow.png"));
    helpAction->setIcon(QIcon("icons/help.png"));

    saveAction->setEnabled(false);
    saveAsAction->setEnabled(false);
    deleteRowAction->setEnabled(false);
    copyDownAction->setEnabled(false);
    addRowAction->setEnabled(false);
    addColumnAction->setEnabled(false);
    deleteColumnAction->setEnabled(false);
    renameAction->setEnabled(false);
    openAudioWindowAction->setEnabled(false);
    openTrackPropertiesAction->setEnabled(false);

    newAction->setShortcut(QKeySequence::New);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    quitAction->setShortcut(QKeySequence::Quit);
    helpAction->setShortcut(QKeySequence::HelpContents);
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
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
    editMenu->addAction(addRowAction);
    editMenu->addAction(deleteRowAction);
    editMenu->addAction(addColumnAction);
    editMenu->addAction(deleteColumnAction);
    editMenu->addAction(copyDownAction);
    gridMenu->addAction(openTrackPropertiesAction);
    gridMenu->addAction(openAudioWindowAction);
    aboutMenu->addAction(helpAction);
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
	//grid = new ChordTableWidget(); //Fenere d'accords

	editionSelector = new EditionSelector(this);

    layout = new QGridLayout();
	layout->addWidget(editionSelector, 0, 1);
	layout->addWidget(chordTree, 0, 0); //Liste des accords en haut-gauche
//	layout->addWidget(p, 0, 1);
    centralArea->setLayout(layout);
}

void GridEditor::createGrid(int columns, int rows)
{
	grid = new ChordTableWidget(columns, rows, this);

	connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
	connect(grid, SIGNAL(play(int)), this, SIGNAL(play(int)));
	connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
	connect(chordTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
	connect(addRowAction, SIGNAL(triggered()), grid, SLOT(insert_row()));
	connect(addColumnAction, SIGNAL(triggered()), grid, SLOT(insert_column()));
	connect(deleteColumnAction, SIGNAL(triggered()), grid, SLOT(delete_selected_column()));
	connect(copyDownAction, SIGNAL(triggered()), grid, SLOT(copy_down_rows()));
	connect(deleteRowAction, SIGNAL(triggered()), grid, SLOT(delete_selected_row()));


	saveAction->setEnabled(true);
	saveAsAction->setEnabled(true);
	addRowAction->setEnabled(true);
	addColumnAction->setEnabled(true);
	renameAction->setEnabled(true);
	openAudioWindowAction->setEnabled(true);
	openTrackPropertiesAction->setEnabled(true);
	deleteRowAction->setEnabled(true);
	deleteColumnAction->setEnabled(true);
	copyDownAction->setEnabled(true);
}

/**
 * @brief GridEditor::connectActionToSlot
 *
 * Défini les relations entre signaux et slots pour la fenêtre principale.
 */
void GridEditor::connectActionToSlot()
{
    connect(trackProperties, SIGNAL(trackChanged()), this, SLOT(setStatusText()));
    connect(trackProperties, SIGNAL(artistChanged()), this, SLOT(setStatusText()));

    connect(newAction, SIGNAL(triggered()), this, SLOT(newGrid()));
    connect(renameAction, SIGNAL(triggered()), this, SLOT(rename()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(toXML()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	connect(openAction, SIGNAL(triggered()), this, SLOT(fromXML()));
    connect(openAudioWindowAction, SIGNAL(triggered()), audioWindow, SLOT(show()));
	connect(openTrackPropertiesAction, SIGNAL(triggered()), trackProperties, SLOT(exec()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));



    connect(this, SIGNAL(play(int)), audioWindow, SLOT(playFrom(int)));
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
}

/**
 * @brief GridEditor::newGrid
 *
 * Mise en place d'une nouvelle grille dans le widget central.
 */
void GridEditor::firstNewGrid()
{
    if(!saveBeforeQuit())
        return;
	if(grid != 0)
	{
		delete grid;
	}
	createGrid(newGridDialog->getColumns() + 1, newGridDialog->getLines());

	trackProperties->setTrack(newGridDialog->getTrack());
	trackProperties->setArtist(newGridDialog->getArtist());
	trackProperties->setBarSize(newGridDialog->getBarSize());
	layout->removeWidget(editionSelector);

	delete editionSelector;

	editorPanel = new EditorPanel(grid, audioWindow, this);
	layout->addWidget(editorPanel, 0, 1);

	isGridSet = true;
}


void GridEditor::newGrid()
{
	if(!saveBeforeQuit())
		return;

	newGridDialog = new NewGridDialog(this);
    filename = "";

	int accept = newGridDialog->exec();
	if(accept == QDialog::Rejected)
		return;
	else
	{
		if(grid != 0)
		{
			delete grid;
		}
		createGrid(newGridDialog->getColumns() + 1, newGridDialog->getLines());

		trackProperties->setTrack(newGridDialog->getTrack());
		trackProperties->setArtist(newGridDialog->getArtist());
		trackProperties->setBarSize(newGridDialog->getBarSize());

		editorPanel = new EditorPanel(grid, audioWindow, this);
		layout->addWidget(editorPanel, 0, 1);

        if(!isGridSet) {
            layout->removeWidget(editionSelector);
            delete editionSelector;
        }

		isGridSet = true;
	}
	delete newGridDialog;
}

/**
 * @brief GridEditor::newEditor
 * @param type Type d'éditeur à créer
 *
 * Demande la création de l'éditeur demandé (utilisé depuis l'assistant).
 */
void GridEditor::startGrid(int type)
{
    if(type == NEW_GRID)
        newGrid();
    else if(type == OPEN_GRID)
        fromXML();
}

/**
 * @brief GridEditor::toXML
 * @param filename Laisser vide pour demander où faire l'enregistrement, ou donner le chemin vers le fichier dans lequel écrire
 *
 * Slot pour la sauvegarde d'un fichier XML.
 */
// penser à faire en sorte que les propriétés de base du morceau soient obligatoires (nom, etc...)
void GridEditor::toXML(QString filename) //ça serait bien qu'on sélectionne le fichier ou on sauve.
{
	// Pour tester qu'on sauve bien un xml
	int pos = 0;
	QRegExp xmlExt("\\S+\\.xml");
	QRegExpValidator *validator = new QRegExpValidator(xmlExt, this);

    if(filename == "")
        filename = QFileDialog::getSaveFileName(this, "Sauvegarde", ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);

    if(validator->validate(filename, pos) != QValidator::Acceptable)
        filename.append(".xml");

	LogicalTrack* track = grid->getLogicalTrack();

	track->setTrackName(trackProperties->getTrack());
	track->setArtist(trackProperties->getArtist());
	track->setMesure(trackProperties->getBarSize());
	track->setLine(grid->rowCount());
	track->setColumn(grid->columnCount() - 1);
	track->setAudioFileName(audioWindow->getFilename()); //vérifier si chemin absolu

    TrackLoader::convertLogicalTrackToXml(track, filename);
    delete validator;
}

/**
 * @brief GridEditor::fromXML
 *
 * Slot pour le chargement d'un fichier XML.
 */
void GridEditor::fromXML() //ça serait bien qu'on sélectionne le fichier ou on sauve.
{
    if(!saveBeforeQuit())
        return;
	LogicalTrack* track = new LogicalTrack;

    QString file = QFileDialog::getOpenFileName(this, tr("Loading"), ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);
    if(file == "")
        return;
    filename = file;
	TrackLoader::convertXmlToLogicalTrack(file, track); //tester la valeur de retour et afficher dialog si échec

	trackProperties->setTrack(track->getTrackName());
	trackProperties->setArtist(track->getArtist());
	status->showMessage(statusText());

	audioWindow->setAudioFileName(track->getAudioFileName()); //vérifier si chemin absolu
	audioWindow->setAudioFile();

	// il faudra penser à recalculer le début, la fin et la durée de la première mesure pour les mettre
	// dans audiowindow

	if(grid != 0)
	{
		delete grid;
	}
	createGrid(track->getColumn() + 1, track->getLine());

    grid->setLogicalTrack(track);

	if(editionSelector != 0)
	{
		layout->removeWidget(editionSelector);
	}

	layout->addWidget(grid, 0, 1);

   isGridSet = true;

    delete track;
}

/**
 * @brief GridEditor::about
 *
 * Affiche une fenêtre de dialogue "A propos"
 */
void GridEditor::about()
{
	QMessageBox::information(this, tr("About GridEditor"), tr("Editor for the GuitarTutor software suite.")); //TODO
}

/**
 * @brief GridEditor::statusText
 * @return Texte à afficher dans la barre de status
 *
 * Renvoie le texte à afficher dans la barre de status (morceau, artiste).
 */
QString GridEditor::statusText()
{
	QString text = "Morceau: ";
	text += trackProperties->getTrack();
	text += ". Artiste: ";
	text += trackProperties->getArtist();
	text += ".";
	return (text == "Morceau: . Artiste: .")? "" : text; //cette ligne est quand même un peu sale... j'ai honte
}


/**
 * @brief GridEditor::setStatusText
 *
 * Slot pour changer le texte du status.
 */
void GridEditor::setStatusText()
{
	statusInfo->setText(statusText());
}

/**
 * @brief GridEditor::rename
 * @todo TODO
 */
void GridEditor::rename()
{

}

/**
 * @brief GridEditor::saveBeforeQuit
 * @return true si et seulement si l'utilisateur a choisi de continuer
 *
 * Se déclenche lorsque l'utilisateur veut changer de grille. Cette fonction vérifie que la grille courante a été sauvegardée,
 * auquel cas il est invité à effectuer la sauvegarde ou à annuler son action.
 */
bool GridEditor::saveBeforeQuit()
{
    if (isGridSet)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The document has been modified"));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        switch (msgBox.exec())
        {
            case QMessageBox::Save:
                saveAction->activate(QAction::Trigger);
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return false;
                break;
            default:
                return false;
                break;
        }
    }
    return true;
}

/**
 * @brief GridEditor::save
 *
 * Slot servant à rediriger vers l'enregistrement à partir d'une demande de type "enregistrer" (et non "enregistrer sous")
 */
void GridEditor::save()
{
    toXML(filename);
}

void GridEditor::help()
{
    HelpWindow helpWindow;
    helpWindow.exec();
}
