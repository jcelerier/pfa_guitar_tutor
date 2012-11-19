/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"

/**
 *
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
    editionSelector = new EditionSelector();
    editionSelector->show();
}

GridEditor::~GridEditor() {
    delete grid;
    delete chordTree;
    delete openAction;
    delete saveAction;
    delete addRowAction;
    delete copyDownAction;
    delete deleteRowAction;
    delete layout;
}

/**
 * Création de la liste d'accords permettant le choix à l'utilisateur lors de la saisi d'une grille
 */
void GridEditor::buildChordTree() {
    chordTree = new QTreeWidget();
    chordTree->setFixedWidth(200);
    chordTree->setHeaderLabel(tr("Chord choice"));
    chordTree->setDisabled(true); //Désactivé par défaut tant que new_button n'a pas été déclenché

    /*Ajout des cordes*/
    //TODO: une optimisation est évidente... Mettre les accords dans un enum par exemple, puis boucler dessus
    chordTree->addTopLevelItem(buildChord("C"));
    chordTree->addTopLevelItem(buildChord("C#"));
    chordTree->addTopLevelItem(buildChord("Db"));
    chordTree->addTopLevelItem(buildChord("D"));
    chordTree->addTopLevelItem(buildChord("D#"));
    chordTree->addTopLevelItem(buildChord("Eb"));
    chordTree->addTopLevelItem(buildChord("E"));
    chordTree->addTopLevelItem(buildChord("F"));
    chordTree->addTopLevelItem(buildChord("F#"));
    chordTree->addTopLevelItem(buildChord("Gb"));
    chordTree->addTopLevelItem(buildChord("G"));
    chordTree->addTopLevelItem(buildChord("G#"));
    chordTree->addTopLevelItem(buildChord("Ab"));
    chordTree->addTopLevelItem(buildChord("A"));
    chordTree->addTopLevelItem(buildChord("A#"));
    chordTree->addTopLevelItem(buildChord("Bb"));
    chordTree->addTopLevelItem(buildChord("B"));
}

/**
 * @brief GridEditor::build_chord
 * @param base_name Tonalité à rajouter à la liste des accords.
 * @return L'élément à rajouter à la liste des accords.
 *
 * Cette fonction crée un objet contenant les différentes possibilités d'accords à partir d'une tonalité donnée: mineur, majeur,...
 */
QTreeWidgetItem* GridEditor::buildChord(const QString tonality) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, tonality);

    QTreeWidgetItem* children[6];
    QString tuning[6] = {"", "m", "aug", "dim", "sus4", "sus2"};
    for(int i=0; i<6; i++) {
        children[i] = new QTreeWidgetItem();
        children[i]->setText(0, tonality + tuning[i]);
        item->addChild(children[i]);
    }
    return item;
}

void GridEditor::changeState() {
    if (grid->is_selection_empty() && chordTree->isEnabled())
        chordTree->setEnabled(false);
    else if(!grid->is_selection_empty() && !chordTree->isEnabled())
        chordTree->setEnabled(true);
    if (grid->is_row_selected() && !deleteRowAction->isEnabled()) {
        deleteRowAction->setEnabled(true);
        copyDownAction->setEnabled(true);
    }
    else if (!grid->is_row_selected() && deleteRowAction->isEnabled()) {
        deleteRowAction->setEnabled(false);
        copyDownAction->setEnabled(false);
    }
}

/**
 * @brief GridEditor::importXml
 *
 * Chargement d'une grille pré-enregistrée.
 */

void GridEditor::importXml() {
    QString xgrid_file = QFileDialog::getOpenFileName(this, tr("Open chords grid"), ".", tr("xgrid Files (*.xgrid)"));
    if (xgrid_file.isNull())
        return;
    delete grid;
    grid = new ChordTableWidget(xgrid_file);
    layout->addWidget(grid, 0, 1);
    title->setText(grid->get_name());
    saveAction->setEnabled(true);
    addRowAction->setEnabled(true);
    renameAction->setEnabled(true);
    connect(chordTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
    connect(addRowAction, SIGNAL(triggered()), grid, SLOT(insert_row()));
    connect(copyDownAction, SIGNAL(triggered()), grid, SLOT(copy_down_rows()));
    connect(deleteRowAction, SIGNAL(triggered()), grid, SLOT(delete_selected_row()));
}

/**
 * @brief GridEditor::exportXml
 *
 * Récupération de la grille éditée pour exportation.
 */

void GridEditor::exportXml() {
    //if (grid->get_name().isEmpty()) {
        bool ok;
        QString name;
        do {
            name = QInputDialog::getText(this, tr("Grid name"), tr("Name the current grid to save it.\n\n Name:"), QLineEdit::Normal, "", &ok);
            if (!ok)
                return;
            if (name.isEmpty()) {
                QMessageBox mb;
                mb.setWindowTitle(tr("Warning"));
                mb.setText(tr("You have to enter a name to continue."));
                mb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                int return_value = mb.exec();
                if (return_value == QMessageBox::Cancel)
                    return;
            }
        } while (name.isEmpty());
        //grid->set_name(name);
    //}
    grid->to_xml(name + ".xgrid");
    QMessageBox mb;
    mb.setWindowTitle(tr("Grid saved"));
    mb.setText(tr("The chords grid is saved at ./") + name + ".xgrid");
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

/**
 * @brief GridEditor::newGrid
 *
 * Mise en place d'une nouvelle grille dans le widget central.
 */

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
    grid = new ChordTableWidget(column + 1, 10);
    layout->addWidget(grid, 0, 1);
    saveAction->setEnabled(true);
    addRowAction->setEnabled(true);
    renameAction->setEnabled(true);
    connect(chordTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
    connect(addRowAction, SIGNAL(triggered()), grid, SLOT(insert_row()));
    connect(copyDownAction, SIGNAL(triggered()), grid, SLOT(copy_down_rows()));
    connect(deleteRowAction, SIGNAL(triggered()), grid, SLOT(delete_selected_row()));
}

/**
 * @brief GridEditor::rename
 *
 * Action pour renommer un fichier xgrid.
 */
void GridEditor::rename() {
    bool ok;
    QString name = QInputDialog::getText(this, tr("Grid name"), tr("Name:"), QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;
    title->setText(name);
    grid->set_name(name);
}

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
    copyDownAction = new QAction(tr("&Copy down"), this);
    renameAction = new QAction(tr("Rename"), this);

    quitAction->setIcon(QIcon("icons/quit.png"));
    aboutAction->setIcon(QIcon("icons/about.png"));
    newAction->setIcon(QIcon("icons/new.png"));
    saveAction->setIcon(QIcon("icons/save.png"));
    openAction->setIcon(QIcon("icons/open.png"));
    addRowAction->setIcon(QIcon("icons/addrow.png"));
    deleteRowAction->setIcon(QIcon("icons/deleterow.png"));

    saveAction->setEnabled(false);
    deleteRowAction->setEnabled(false);
    copyDownAction->setEnabled(false);
    addRowAction->setEnabled(false);
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
    fileMenu->addAction(quitAction);
    editMenu->addAction(addRowAction);
    editMenu->addAction(deleteRowAction);
    editMenu->addAction(copyDownAction);
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
}

/**
 * @brief GridEditor::createCentralWidget
 *
 * Création du widget principale de la fenêtre.
 */
void GridEditor::createCentralWidget() {
    centralArea = new QWidget();

    /*Mise en place du layout*/
    buildChordTree(); //Initialisation de chord_tree
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
void GridEditor::connectActionToSlot(){
    connect(chordTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(changeState()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(exportXml()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(importXml()));
    connect(addRowAction, SIGNAL(triggered()), grid, SLOT(insert_row()));
    connect(copyDownAction, SIGNAL(triggered()), grid, SLOT(copy_down_rows()));
    connect(deleteRowAction, SIGNAL(triggered()), grid, SLOT(delete_selected_row()));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newGrid()));
    connect(renameAction, SIGNAL(triggered()), this, SLOT(rename()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}


