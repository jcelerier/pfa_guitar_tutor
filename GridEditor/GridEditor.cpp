/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"
#include <QtWidgets/QFileDialog>


/**
 * @brief GridEditor::GridEditor
 *
 * Constructeur de la grille d'accords.
 */
GridEditor::GridEditor(): m_trackProperties(new TrackProperties(this)), m_saveQueue(new UndoManager(this))
{
	m_isPanelSet = false;
	m_editorPanel = 0;
	m_grid = 0;
	m_helpWindow = 0;
	m_audioWindow = new AudioWindow(this);

	m_status = statusBar();
	m_statusInfo = new QLabel(statusText(), m_status);
	m_status->addPermanentWidget(m_statusInfo, 1);
	m_status->setSizeGripEnabled(false);

	m_editorPanel = new EditorPanel(m_grid, m_audioWindow, m_trackProperties, this);

	setWindowTitle("GridEditor");
	resize(800, 600); //Taille de la fenêtre
	createMenu();
	createActions();
	setActionsToMenu();
	createToolbar();

	createCentralWidget();
	setCentralWidget(m_centralArea);
	connectActionToSlot();

	m_settings = new QSettings("GuitarTutor", "GridEditor"); //Permet de retenir la configuration du logiciel
	connect(m_trackProperties, SIGNAL(timeSignatureChanged(int)), m_audioWindow, SIGNAL(timeSignatureChanged(int)));

	connect(qApp, SIGNAL(aboutToQuit(QPrivateSignal)), this, SLOT(saveBeforeQuit()));

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
	delete m_fileMenu; delete m_editMenu;  delete m_aboutMenu;
	delete m_toolBar;
	delete m_settings;

	delete m_chordTree;
	if(m_grid != 0)
	{
		delete m_grid;
		m_grid = 0;
	}
	delete m_trackProperties;
	delete m_layout;
	delete m_centralArea;
	delete m_statusInfo; delete m_status;
	if(m_helpWindow != 0) {
		delete m_helpWindow;
		m_helpWindow = 0;
	}
	delete m_saveQueue;
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
	m_fileMenu = menuBar()->addMenu(tr("&File"));
	m_editMenu = menuBar()->addMenu(tr("&Edit"));
	m_aboutMenu = menuBar()->addMenu(tr("&About"));
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
	m_fileMenu->addAction(newAction);
	m_fileMenu->addAction(openAction);
	m_fileMenu->addAction(saveAction);
	m_fileMenu->addAction(saveAsAction);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(quitAction);
	m_editMenu->addAction(undoAction);
	m_editMenu->addAction(redoAction);
	m_editMenu->addSeparator();
	m_editMenu->addAction(addRowAction);
	m_editMenu->addAction(deleteRowAction);
	m_editMenu->addAction(addColumnAction);
	m_editMenu->addAction(deleteColumnAction);
	//editMenu->addAction(copyDownAction); //trop buggué
	m_aboutMenu->addAction(helpAction);
	m_aboutMenu->addAction(aboutAction);
}

/**
 * @brief GridEditor::createToolbar
 *
 * Crée la barre d'outil principale et y ajoute les actions.
 */
void GridEditor::createToolbar() {
	m_toolBar = new QToolBar(tr("Tool bar"));
	addToolBar(Qt::RightToolBarArea, m_toolBar);
	m_toolBar->addAction(newAction);
	m_toolBar->addAction(openAction);
	m_toolBar->addAction(saveAction);
	m_toolBar->addSeparator();
	m_toolBar->addAction(addRowAction);
	m_toolBar->addAction(deleteRowAction);
	m_toolBar->addAction(addColumnAction);
	m_toolBar->addAction(deleteColumnAction);
}

/**
 * @brief GridEditor::createCentralWidget
 *
 * Création du widget principale de la fenêtre.
 */
void GridEditor::createCentralWidget() {
	m_centralArea = new QWidget();

	/*Mise en place du layout*/
	m_chordTree = new ChordTree(); //Initialisation de chord_tree

	m_editionSelector = new EditionSelector(this);

	m_layout = new QGridLayout();
	m_layout->addWidget(m_editionSelector, 0, 1);
	m_layout->addWidget(m_chordTree, 0, 0); //Liste des accords en haut-gauche

	m_centralArea->setLayout(m_layout);
}

/**
 * @brief GridEditor::createGrid
 * @param columns Colonnes
 * @param rows Lignes
 *
 * Instancie une nouvelle grille, et crée les widgets, actions, connections, etc... correspondants
 */
void GridEditor::createGrid(int columns, int rows)
{
	if(m_grid != 0)
	{
		m_grid->disconnect();
		delete m_grid;
		m_grid = 0;
	}
	m_grid = new ChordTableWidget(columns, rows, this);

	connect(m_grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
	connect(m_grid, SIGNAL(play(int)), this, SIGNAL(play(int)));

	connect(this, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)), m_grid, SLOT(setTimeInfo(QTime,QTime,QTime)));
	connect(this, SIGNAL(sigTimeData(QTime)), m_grid, SLOT(isPlayingAt(QTime)));

	connect(m_trackProperties, SIGNAL(barsizeChanged(int)), m_grid, SIGNAL(barsizeChanged(int)));

	connect(m_chordTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), m_grid, SLOT(fill_selection(QTreeWidgetItem*,int)));


	connect(addRowAction, SIGNAL(triggered()), m_grid, SLOT(insertChordRow()));
	connect(addColumnAction, SIGNAL(triggered()), m_grid, SLOT(insertChordColumn()));
	connect(deleteColumnAction, SIGNAL(triggered()), m_grid, SLOT(deleteSelectedColumn()));
	//connect(copyDownAction, SIGNAL(triggered()), grid, SLOT(copyDownRows()));
	connect(deleteRowAction, SIGNAL(triggered()), m_grid, SLOT(deleteSelectedRow()));

	saveAction->setEnabled(true);
	saveAsAction->setEnabled(true);
	addRowAction->setEnabled(true);
	addColumnAction->setEnabled(true);
	openAudioWindowAction->setEnabled(true);
	openTrackPropertiesAction->setEnabled(true);
	deleteRowAction->setEnabled(true);
	deleteColumnAction->setEnabled(true);
	copyDownAction->setEnabled(true);

	m_editorPanel->updateGrid(m_grid);

	if(!m_isPanelSet)
	{
		if(m_editionSelector != 0)
		{
			m_layout->removeWidget(m_editionSelector);
			delete m_editionSelector;
			m_editionSelector = 0;
		}

		m_layout->addWidget(m_editorPanel, 0, 1);

	}
	m_isPanelSet = true;
}

/**
 * @brief GridEditor::connectActionToSlot
 *
 * Définit les relations entre signaux et slots pour la fenêtre principale.
 */
void GridEditor::connectActionToSlot()
{
	connect(m_trackProperties, SIGNAL(trackChanged()), this, SLOT(setStatusText()));
	connect(m_trackProperties, SIGNAL(artistChanged()), this, SLOT(setStatusText()));
	connect(this, SIGNAL(propsChanged()), this, SLOT(setStatusText()));

	connect(newAction, SIGNAL(triggered()), this, SLOT(newGrid()));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(toXML()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	connect(openAction, SIGNAL(triggered()), this, SLOT(fromXML()));
	connect(openAudioWindowAction, SIGNAL(triggered()), m_audioWindow, SLOT(show()));
	connect(openTrackPropertiesAction, SIGNAL(triggered()), m_trackProperties, SLOT(exec()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));

	connect(undoAction, SIGNAL(triggered()), m_saveQueue, SLOT(undo()));
	connect(redoAction, SIGNAL(triggered()), m_saveQueue, SLOT(redo()));

	connect(this, SIGNAL(play(int)), m_audioWindow, SLOT(playFrom(int)));
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
	if (m_grid->isSelectionEmpty() &&
		m_chordTree->isEnabled())
	{
		m_chordTree->setEnabled(false);
	}
	else if(!m_grid->isSelectionEmpty() &&
			!m_chordTree->isEnabled())
	{
		m_chordTree->setEnabled(true);
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

	m_trackProperties->setTrack(m_newGridDialog->getTrack());
	m_trackProperties->setArtist(m_newGridDialog->getArtist());
	m_trackProperties->setBarSize(m_newGridDialog->getBarSize());

	m_barsize = m_trackProperties->getBarSize();

	createGrid(m_newGridDialog->getColumns() + 1, m_newGridDialog->getLines());

	m_saveQueue->firstSave();
	connect(m_grid, SIGNAL(somethingChanged()), m_saveQueue, SIGNAL(save()));
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

	m_newGridDialog = new NewGridDialog(this);
	m_filename = "";

	int accept = m_newGridDialog->exec();
	if(accept == QDialog::Rejected)
	{
		return;
	}
	else
	{
		m_trackProperties->setTrack(m_newGridDialog->getTrack());
		m_trackProperties->setArtist(m_newGridDialog->getArtist());
		m_trackProperties->setBarSize(m_newGridDialog->getBarSize());
		m_barsize = m_trackProperties->getBarSize();

		m_saveQueue->clear();
		createGrid(m_newGridDialog->getColumns() + 1, m_newGridDialog->getLines());
		m_saveQueue->firstSave();
		connect(m_grid, SIGNAL(somethingChanged()), m_saveQueue, SIGNAL(save()));
	}
	delete m_newGridDialog;

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
		track = m_grid->getLogicalTrack();
	}
	catch(QException& e)
	{
		return;
	}
	track->setTrackName(m_trackProperties->getTrack());
	track->setArtist(m_trackProperties->getArtist());
	track->setMesure(m_trackProperties->getBarSize());
	track->setComment(m_trackProperties->getComment());
	track->setTimeSignature(m_trackProperties->getTimeSignature());
	track->setLine(m_grid->rowCount());
	track->setColumn(m_grid->columnCount() - 1);
	track->setAudioFileName(m_audioWindow->getFilename()); //vérifier si chemin absolu
	track->setBars(m_audioWindow->getBar(), m_audioWindow->getBeginning(), m_audioWindow->getEnd());


	TrackLoader::convertLogicalTrackToXml(track, filename);

	m_filename = filename;
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
	m_filename = file; // ici seulement on a chargé un fichier valide

	m_trackProperties->setTrack(track->getTrackName());
	m_trackProperties->setArtist(track->getArtist());
    m_trackProperties->setBarSize(track->getChordsPerBar());
	m_barsize = m_trackProperties->getBarSize();
	m_trackProperties->setComment(track->getComment());
	m_trackProperties->setTimeSignature(track->getTimeSignature());

	m_audioWindow->setAudioFileName(track->getAudioFileName());
	m_audioWindow->setAudioFile();
	m_audioWindow->setBar(track->getBar());
	m_audioWindow->setBeginning(track->getBeginning());
	m_audioWindow->setEnd(track->getEnd());

	//nope à cause du mod +1... : connect(audioWindow, SIGNAL(somethingChanged()), saveQueue, SIGNAL(save()));

	m_saveQueue->clear();
	createGrid(track->getColumn() + 1, track->getLine());
	m_grid->setLogicalTrack(track);
	m_saveQueue->firstSave();
	connect(m_grid, SIGNAL(somethingChanged()), m_saveQueue, SIGNAL(save()));

    emit m_trackProperties->barsizeChanged(track->getChordsPerBar());

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
	if(! m_trackProperties->getTrack().isEmpty() )
	{
		text += tr("Track: ");
		text += m_trackProperties->getTrack();
		text += ". ";
	}
	if(! m_trackProperties->getArtist().isEmpty() )
	{
		text += tr("  Artist: ");
		text += m_trackProperties->getArtist();
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
	m_statusInfo->setText(statusText());
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
	if (m_isPanelSet)
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
	toXML(m_filename);
}

/**
 * @brief GridEditor::help
 *
 * Affiche la fenêtre d'aide
 */
void GridEditor::help()
{
	if(m_helpWindow != 0)
		delete m_helpWindow;
	m_helpWindow = new HelpWindow(this);
	m_helpWindow->exec();
}



void GridEditor::enableUndo(bool b)
{
	undoAction->setEnabled(b);
}

void GridEditor::enableRedo(bool b)
{
	redoAction->setEnabled(b);
}


void GridEditor::closeEvent(QCloseEvent *event)
{
	if(!saveBeforeQuit())
	{
		event->ignore();
	}
}
