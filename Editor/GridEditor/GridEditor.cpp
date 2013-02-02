/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"
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

    connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
    connect(this, SIGNAL(play(int)), audioWindow, SLOT(playFrom(int)));

    settings = new QSettings("GuitarTutor", "GridEditor"); //Permet de retenir la configuration du logiciel
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
    delete fileMenu; delete editMenu; delete optionMenu; delete gridMenu; delete aboutMenu;
    delete toolBar;
    delete openAction; delete saveAction; delete addRowAction; delete copyDownAction; delete deleteRowAction;
		delete quitAction; delete aboutAction; delete newAction; delete renameAction; delete openAudioWindowAction;
		delete openTrackPropertiesAction; //peut être faire un dictionnaire d'actions qu'on puisse appeler par leur nom.
		//ex. : actions["new"];
    delete settings;
    delete centralArea;
    delete editionSelector;
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
    deleteRowAction->setEnabled(false);
    copyDownAction->setEnabled(false);
    addRowAction->setEnabled(false);
    addColumnAction->setEnabled(false);
    deleteColumnAction->setEnabled(false);
    renameAction->setEnabled(false);
    openAudioWindowAction->setEnabled(false);
    openTrackPropertiesAction->setEnabled(false);
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
    fileMenu->addAction(quitAction);
    editMenu->addAction(addRowAction);
    editMenu->addAction(deleteRowAction);
    editMenu->addAction(addColumnAction);
    editMenu->addAction(deleteColumnAction);
    editMenu->addAction(copyDownAction);
    gridMenu->addAction(openTrackPropertiesAction);
    gridMenu->addAction(openAudioWindowAction);
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
	editionSelector = new EditionSelector(this);

    layout = new QGridLayout();
	layout->addWidget(editionSelector, 0, 1);
	layout->addWidget(chordTree, 0, 0); //Liste des accords en haut-gauche
//	layout->addWidget(p, 0, 1);
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
	connect(trackProperties, SIGNAL(trackChanged()), this, SLOT(setStatusText()));
	connect(trackProperties, SIGNAL(artistChanged()), this, SLOT(setStatusText()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
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
    if(!saveBeforeQuit())
        return;

	newGridDialog = new NewGridDialog(this);

	int accept = newGridDialog->exec();
	if(accept == QDialog::Rejected)
		return;
    else
	{
        delete grid;
        grid = new ChordTableWidget(newGridDialog->getColumns() + 1, newGridDialog->getLines(), this);
        connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
		connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));

        trackProperties->setTrack(newGridDialog->getTrack());
        trackProperties->setArtist(newGridDialog->getArtist());
        trackProperties->setBarSize(newGridDialog->getBarSize());
        layout->removeWidget(editionSelector);
		delete editionSelector;

		editorPanel = new EditorPanel(grid, audioWindow, this);
		layout->addWidget(editorPanel, 0, 1);

        saveAction->setEnabled(true);
		addRowAction->setEnabled(true);
		addColumnAction->setEnabled(true);
		renameAction->setEnabled(true);
        openAudioWindowAction->setEnabled(true);
        openTrackPropertiesAction->setEnabled(true);
        deleteRowAction->setEnabled(true);
        deleteColumnAction->setEnabled(true);
        copyDownAction->setEnabled(true);

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
    if(!saveBeforeQuit())
        return;
	LogicalTrack* track = new LogicalTrack;

    QString file = QFileDialog::getOpenFileName(this, tr("Loading"), ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);
    if(file == "")
        return;
	TrackLoader::convertXmlToLogicalTrack(file, track); //tester la valeur de retour et afficher dialog si échec

	trackProperties->setTrack(track->getTrackName());
	trackProperties->setArtist(track->getArtist());
	status->showMessage(statusText());

	audioWindow->setAudioFileName(track->getAudioFileName()); //vérifier si chemin absolu
	audioWindow->setAudioFile();

	// il faudra penser à recalculer le début, la fin et la durée de la première mesure pour les mettre
	// dans audiowindow
    delete grid;
    grid = new ChordTableWidget(4+1, 8, this); //TODO
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
    grid->setLogicalTrack(track);
    layout->removeWidget(editionSelector);
    layout->addWidget(grid, 0, 1);

    saveAction->setEnabled(true);
    addRowAction->setEnabled(true);
    addColumnAction->setEnabled(true);
    renameAction->setEnabled(true);
    openAudioWindowAction->setEnabled(true);
    openTrackPropertiesAction->setEnabled(true);
    deleteRowAction->setEnabled(true);
    deleteColumnAction->setEnabled(true);
    copyDownAction->setEnabled(true);

    isGridSet = true;
}

/**
 * @brief GridEditor::about
 *
 * Affiche une fenêtre de dialogue "A propos"
 */
void GridEditor::about()
{
    QMessageBox::information(this, tr("About GridEditor"), tr("Lorem ipsum dolor sit amet, consectetur adipiscing elit.")); //TODO
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
