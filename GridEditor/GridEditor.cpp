/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"
#include <QtWidgets/QFileDialog>
#include <QDebug>

/**
 * @brief GridEditor::GridEditor
 *
 * Constructeur de la grille d'accords.
 */
GridEditor::GridEditor(): trackProperties(new TrackProperties(this)), saveQueue(new SaveQueue(this))
{
	isPanelSet = false;
	editorPanel = 0;
	grid = 0;
	helpWindow = 0;
	//trackProperties = new TrackProperties(this);
	audioWindow = new AudioWindow(this);

	status = statusBar();
	statusInfo = new QLabel(statusText(), status);
	status->addPermanentWidget(statusInfo, 1);
	status->setSizeGripEnabled(false);

	editorPanel = new EditorPanel(grid, audioWindow, trackProperties, this);

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
	connect(trackProperties, SIGNAL(timeSignatureChanged(int)), audioWindow, SIGNAL(timeSignatureChanged(int)));
//	connect(trackProperties, SIGNAL(somethingChanged()), saveQueue, SIGNAL(save()));
}

/**
 * @brief GridEditor::~GridEditor
 *
 * Destructeur de la grille d'accords.
 */
GridEditor::~GridEditor() {
	delete openAction; delete saveAction; delete addRowAction; delete copyDownAction; delete deleteRowAction; delete saveAsAction;
	delete quitAction; delete aboutAction; delete newAction;  delete openAudioWindowAction;
	delete openTrackPropertiesAction; delete addColumnAction; delete deleteColumnAction; delete helpAction;
	//peut être faire un dictionnaire d'actions qu'on puisse appeler par leur nom.
	//ex. : actions["new"];
	delete undoAction; delete redoAction;
	delete fileMenu; delete editMenu;  delete aboutMenu;
	delete toolBar;
	delete settings;

	delete chordTree;
	if(grid != 0)
	{
		delete grid;
		grid = 0;
	}
	delete trackProperties;
	delete layout;
	delete centralArea;
	delete statusInfo; delete status;
	if(helpWindow != 0) {
		delete helpWindow;
		helpWindow = 0;
	}
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
	openAudioWindowAction = new QAction(tr("Audio"), this);
	openTrackPropertiesAction = new QAction(tr("Properties"), this);
	helpAction = new QAction(tr("&Help"), this);

	undoAction = new QAction(tr("&Undo"), this);
	redoAction = new QAction(tr("&Redo"), this);

	quitAction->setIcon(QIcon(":/icons/quit.png"));
	aboutAction->setIcon(QIcon(":/icons/about.png"));
	newAction->setIcon(QIcon(":/icons/new.png"));
	saveAction->setIcon(QIcon(":/icons/save.png"));
	openAction->setIcon(QIcon(":/icons/open.png"));
	addRowAction->setIcon(QIcon(":/icons/addrow.png"));
	deleteRowAction->setIcon(QIcon(":/icons/deleterow.png"));
	addColumnAction->setIcon(QIcon(":/icons/addcolumn.png"));
	deleteColumnAction->setIcon(QIcon(":/icons/deletecolumn.png"));
	helpAction->setIcon(QIcon(":/icons/help.png"));

	saveAction->setEnabled(false);
	saveAsAction->setEnabled(false);
	deleteRowAction->setEnabled(false);
	copyDownAction->setEnabled(false);
	addRowAction->setEnabled(false);
	addColumnAction->setEnabled(false);
	deleteColumnAction->setEnabled(false);
	openAudioWindowAction->setEnabled(false);
	openTrackPropertiesAction->setEnabled(false);
	undoAction->setEnabled(false);
	redoAction->setEnabled(false);

	newAction->setShortcut(QKeySequence::New);
	openAction->setShortcut(QKeySequence::Open);
	saveAction->setShortcut(QKeySequence::Save);
	saveAsAction->setShortcut(QKeySequence::SaveAs);
	quitAction->setShortcut(QKeySequence::Quit);
	helpAction->setShortcut(QKeySequence::HelpContents);
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction->setShortcut(QKeySequence::Redo);
}

/**
 * @brief GridEditor::setActionsToMenu
 *
 * Ajoute les actions au menu.
 */
void GridEditor::setActionsToMenu()
{
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAction);
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addSeparator();
	editMenu->addAction(addRowAction);
	editMenu->addAction(deleteRowAction);
	editMenu->addAction(addColumnAction);
	editMenu->addAction(deleteColumnAction);
	editMenu->addAction(copyDownAction);
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

	editionSelector = new EditionSelector(this);

	layout = new QGridLayout();
	layout->addWidget(editionSelector, 0, 1);
	layout->addWidget(chordTree, 0, 0); //Liste des accords en haut-gauche

	centralArea->setLayout(layout);
}

/**
 * @brief GridEditor::createGrid
 * @param columns Colonnes
 * @param rows Lignes
 *
 * Instancie une nouvelle grille, et crée les widgets, actions, connections, etc... correspondants
 */
void GridEditor::createGrid(int columns, int rows, bool createdFromUndo)
{
	if(grid != 0)
	{
		delete grid;
		grid = 0;
	}
	grid = new ChordTableWidget(columns, rows, this);

	connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
	connect(grid, SIGNAL(play(int)), this, SIGNAL(play(int)));
	if(!createdFromUndo)
	{
		connect(grid, SIGNAL(somethingChanged()), saveQueue, SIGNAL(save()));
	}

	connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
	connect(this, SIGNAL(sigTimeData(QTime)), grid, SLOT(isPlayingAt(QTime)));

	connect(trackProperties, SIGNAL(barsizeChanged(int)), grid, SIGNAL(barsizeChanged(int)));

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
	openAudioWindowAction->setEnabled(true);
	openTrackPropertiesAction->setEnabled(true);
	deleteRowAction->setEnabled(true);
	deleteColumnAction->setEnabled(true);
	copyDownAction->setEnabled(true);

	editorPanel->updateGrid(grid);

	if(!isPanelSet)
	{
		if(editionSelector != 0)
		{
			layout->removeWidget(editionSelector);
			delete editionSelector;
			editionSelector = 0;
		}

		layout->addWidget(editorPanel, 0, 1);
		isPanelSet = true;
	}
}

/**
 * @brief GridEditor::connectActionToSlot
 *
 * Définit les relations entre signaux et slots pour la fenêtre principale.
 */
void GridEditor::connectActionToSlot()
{
	connect(trackProperties, SIGNAL(trackChanged()), this, SLOT(setStatusText()));
	connect(trackProperties, SIGNAL(artistChanged()), this, SLOT(setStatusText()));
	connect(this, SIGNAL(propsChanged()), this, SLOT(setStatusText()));

	connect(newAction, SIGNAL(triggered()), this, SLOT(newGrid()));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(toXML()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	connect(openAction, SIGNAL(triggered()), this, SLOT(fromXML()));
	connect(openAudioWindowAction, SIGNAL(triggered()), audioWindow, SLOT(show()));
	connect(openTrackPropertiesAction, SIGNAL(triggered()), trackProperties, SLOT(exec()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));

	connect(undoAction, SIGNAL(triggered()), saveQueue, SLOT(undo()));
	connect(redoAction, SIGNAL(triggered()), saveQueue, SLOT(redo()));

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
void GridEditor::changeState()
{
	if (grid->is_selection_empty() &&
		chordTree->isEnabled())
	{
		chordTree->setEnabled(false);
	}
	else if(!grid->is_selection_empty() &&
			!chordTree->isEnabled())
	{
		chordTree->setEnabled(true);
	}
}

/**
 * @brief GridEditor::firstNewGrid
 *
 * Mise en place d'une nouvelle grille (action Nouveau) dans le widget central.
 * Méthode spécialement prévue pour le premier lancement, car affiche dans un onglet
 */
void GridEditor::firstNewGrid()
{
	if(!saveBeforeQuit())
		return;

	trackProperties->setTrack(newGridDialog->getTrack());
	trackProperties->setArtist(newGridDialog->getArtist());
	trackProperties->setBarSize(newGridDialog->getBarSize());

	m_barsize = trackProperties->getBarSize();

	createGrid(newGridDialog->getColumns() + 1, newGridDialog->getLines());

	saveQueue->firstSave();
}


/**
 * @brief GridEditor::newGrid
 *
 * Méthode prévue pour les nouvelles grilles (action Nouveau) subséquentes, car affiche dans une fenêtre.
 */
void GridEditor::newGrid()
{
	if(!saveBeforeQuit())
	{
		return;
	}

	newGridDialog = new NewGridDialog(this);
	filename = "";

	int accept = newGridDialog->exec();
	if(accept == QDialog::Rejected)
	{
		return;
	}
	else
	{
		trackProperties->setTrack(newGridDialog->getTrack());
		trackProperties->setArtist(newGridDialog->getArtist());
		trackProperties->setBarSize(newGridDialog->getBarSize());
		m_barsize = trackProperties->getBarSize();

		createGrid(newGridDialog->getColumns() + 1, newGridDialog->getLines());
		saveQueue->firstSave();
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
 * @param filename Laisser vide pour demander où faire l'enregistrement
 *
 * Slot pour la sauvegarde d'un fichier XML.
 */
// penser à faire en sorte que les propriétés de base du morceau soient obligatoires (nom, etc...)
void GridEditor::toXML(QString filename)
{
	// Pour tester qu'on sauve bien un xml
	int pos = 0;
	QRegExp xmlExt("\\S+\\.xml");
	QRegExpValidator *validator = new QRegExpValidator(xmlExt, this);

	if(filename == "")
		filename = QFileDialog::getSaveFileName(this, "Sauvegarde", ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);

	if(validator->validate(filename, pos) != QValidator::Acceptable)
		filename.append(".xml");
	LogicalTrack* track = 0;

	try
	{
		track = grid->getLogicalTrack();
	}
	catch(QException& e)
	{
		return;
	}
	track->setTrackName(trackProperties->getTrack());
	track->setArtist(trackProperties->getArtist());
	track->setMesure(trackProperties->getBarSize());
	track->setComment(trackProperties->getComment());
	track->setTimeSignature(trackProperties->getTimeSignature());
	track->setLine(grid->rowCount());
	track->setColumn(grid->columnCount() - 1);
	track->setAudioFileName(audioWindow->getFilename()); //vérifier si chemin absolu
	track->setBars(audioWindow->getBar(), audioWindow->getBeginning(), audioWindow->getEnd());


	TrackLoader::convertLogicalTrackToXml(track, filename);
	delete validator;
}

void GridEditor::barsizeChanged(int barsize)
{
	m_barsize = barsize;
}

/**
 * @brief GridEditor::fromXML
 *
 * Slot pour le chargement d'un fichier XML.
 */
void GridEditor::fromXML()
{
	if(!saveBeforeQuit())
		return;


	QString file = QFileDialog::getOpenFileName(this, tr("Loading"), ".", tr("XML Files (*.xml)"), 0, QFileDialog::HideNameFilterDetails);
	if(file == "")
		return;

	LogicalTrack* track = new LogicalTrack;
	if(!TrackLoader::convertXmlToLogicalTrack(file, track))
	{
		QMessageBox::information(this, tr("Error"), tr("The file has not been loaded. Is it correct?"));
		return;
	}
	filename = file; // ici seulement on a chargé un fichier valide

	trackProperties->setTrack(track->getTrackName());
	trackProperties->setArtist(track->getArtist());
	trackProperties->setBarSize(track->getMesure());
	m_barsize = trackProperties->getBarSize();
	trackProperties->setComment(track->getComment());
	trackProperties->setTimeSignature(track->getTimeSignature());

	audioWindow->setAudioFileName(track->getAudioFileName()); //vérifier si chemin absolu
	audioWindow->setAudioFile();
	audioWindow->setBar(track->getBar());
	audioWindow->setBeginning(track->getBeginning());
	audioWindow->setEnd(track->getEnd());


	createGrid(track->getColumn() + 1, track->getLine());
	grid->setLogicalTrack(track);

	emit trackProperties->barsizeChanged(track->getMesure());

	saveQueue->firstSave();
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
	QString text;
	if(! trackProperties->getTrack().isEmpty() )
	{
		text += tr("Track: ");
		text += trackProperties->getTrack();
		text += ". ";
	}
	if(! trackProperties->getArtist().isEmpty() )
	{
		text += tr("  Artist: ");
		text += trackProperties->getArtist();
		text += ".";
	}

	return text;
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
 * @brief GridEditor::saveBeforeQuit
 * @return true si et seulement si l'utilisateur a choisi de continuer
 *
 * Se déclenche lorsque l'utilisateur veut changer de grille. Cette fonction vérifie que la grille courante a été sauvegardée,
 * auquel cas il est invité à effectuer la sauvegarde ou à annuler son action.
 */
bool GridEditor::saveBeforeQuit()
{
	if (isPanelSet)
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

/**
 * @brief GridEditor::help
 *
 * Affiche la fenêtre d'aide
 */
void GridEditor::help()
{
	if(helpWindow != 0)
		delete helpWindow;
	helpWindow = new HelpWindow(this);
	helpWindow->exec();
}



void GridEditor::enableUndo(bool b)
{
	undoAction->setEnabled(b);
}

void GridEditor::enableRedo(bool b)
{
	redoAction->setEnabled(b);
}
