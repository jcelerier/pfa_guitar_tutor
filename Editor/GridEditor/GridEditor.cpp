/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"

/**
 *
 */
GridEditor::GridEditor() : QWidget() {
    /*Initialisation de la fenêtre*/

    QDesktopWidget desktop; //Permet de récupérer des informations sur l'écran de l'utilisateur
    this->setBaseSize(3*desktop.width()/4, 3*desktop.height()/4); //Taille de la fenêtre

    /*Mise en place du layout*/
    QFrame* frame = build_frame(); //Barre d'options
    build_chord_tree(); //Initialisation de chord_tree
    grid = new ChordTableWidget(); //Fenere d'accords

    layout = new QGridLayout();
    layout->addWidget(chord_tree, 0, 0); //Liste des accords en haut-gauche
    layout->addWidget(grid, 0, 1); //Fenêtre d'accords en haut-milieu
    layout->addWidget(frame, 0, 2); //Barre d'options en haut-droite
    this->setLayout(layout);

    /*Désactivation des options au démarrage*/
    save_button->setEnabled(false);
    delete_row_button->setEnabled(false);
    copy_down_button->setEnabled(false);
    add_row_button->setEnabled(false);
    rename_button->setEnabled(false);

    /*Connexions signaux-slots pour les boutons du panneau d'options et*/
    connect(chord_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(change_state()));
    connect(save_button, SIGNAL(clicked()), this, SLOT(export_xml()));
    connect(open_button, SIGNAL(clicked()), this, SLOT(import_xml()));
    connect(add_row_button, SIGNAL(clicked()), grid, SLOT(insert_row()));
    connect(copy_down_button, SIGNAL(clicked()), grid, SLOT(copy_down_rows()));
    connect(delete_row_button, SIGNAL(clicked()), grid, SLOT(delete_selected_row()));
    connect(new_button, SIGNAL(clicked()), this, SLOT(new_grid()));
    connect(rename_button, SIGNAL(clicked()), this, SLOT(rename()));
}

GridEditor::~GridEditor() {
    delete grid;
    delete chord_tree;
    delete open_button;
    delete save_button;
    delete add_row_button;
    delete copy_down_button;
    delete delete_row_button;
    delete layout;
}

/**
 * Création de la liste d'accords permettant le choix à l'utilisateur lors de la saisi d'une grille
 */
void GridEditor::build_chord_tree() {
    chord_tree = new QTreeWidget();
    chord_tree->setFixedWidth(200);
    chord_tree->setHeaderLabel(tr("Chord choice"));
    chord_tree->setDisabled(true); //Désactivé par défaut tant que new_button n'a pas été déclenché

    /*Ajout des cordes*/
    //TODO: une optimisation est évidente... Mettre les accords dans un enum par exemple, puis boucler dessus
    chord_tree->addTopLevelItem(build_chord("C"));
    chord_tree->addTopLevelItem(build_chord("C#"));
    chord_tree->addTopLevelItem(build_chord("Db"));
    chord_tree->addTopLevelItem(build_chord("D"));
    chord_tree->addTopLevelItem(build_chord("D#"));
    chord_tree->addTopLevelItem(build_chord("Eb"));
    chord_tree->addTopLevelItem(build_chord("E"));
    chord_tree->addTopLevelItem(build_chord("F"));
    chord_tree->addTopLevelItem(build_chord("F#"));
    chord_tree->addTopLevelItem(build_chord("Gb"));
    chord_tree->addTopLevelItem(build_chord("G"));
    chord_tree->addTopLevelItem(build_chord("G#"));
    chord_tree->addTopLevelItem(build_chord("Ab"));
    chord_tree->addTopLevelItem(build_chord("A"));
    chord_tree->addTopLevelItem(build_chord("A#"));
    chord_tree->addTopLevelItem(build_chord("Bb"));
    chord_tree->addTopLevelItem(build_chord("B"));   
}


/**
 * Création du bloc d'options de l'éditeur.
 * \return Le bloc d'options créé.
 */
QFrame* GridEditor::build_frame() {
    QVBoxLayout* frame_lay = new QVBoxLayout(); //Layout du bloc d'options
    QVBoxLayout* top_frame_lay = new QVBoxLayout(); //Boutons du haut (!)
    QVBoxLayout* bottom_frame_lay = new QVBoxLayout(); //Boutons du bas (!)

    new_button = new QPushButton(tr("New"));
    rename_button = new QPushButton(tr("Rename"));
    open_button = new QPushButton(tr("Open"));
    save_button = new QPushButton(tr("Save"));
    copy_down_button = new QPushButton(tr("Copy down"));
    add_row_button = new QPushButton(tr("Add row"));
    delete_row_button = new QPushButton(tr("Delete row"));

    top_frame_lay->addWidget(new_button);
    top_frame_lay->addWidget(rename_button);
    top_frame_lay->addWidget(open_button);
    top_frame_lay->addWidget(save_button);
    //Les boutons sont sur deux layouts différents, je ne sais pas pourquoi. Autant insérer un séparateur.
    bottom_frame_lay->addWidget(copy_down_button);
    bottom_frame_lay->addWidget(add_row_button);
    bottom_frame_lay->addWidget(delete_row_button);

    frame_lay->addLayout(top_frame_lay); //Regroupement des deux layouts dans le principal
    frame_lay->addLayout(bottom_frame_lay);

    QFrame* frame = new QFrame(); //Bordure autour du bloc
    frame->setFrameStyle(QFrame::Box);
    frame->setLayout(frame_lay);

    QVBoxLayout* principal_frame_lay = new QVBoxLayout(); //Ensemble du bloc
    //TODO: Cette partie ne sert à rien puisqu'il n'y a pas de titre à la fenêtre...
    title = new QLabel();
    title->setFixedHeight(30);
    title->setAlignment(Qt::AlignHCenter);
    QFont title_font;
    title_font.setPointSize(14);
    title_font.setBold(true);
    title->setFont(title_font); //Je rappelle que le titre est vide (sic)...
    principal_frame_lay->addWidget(title);
    //End TODO
    principal_frame_lay->addWidget(frame);

    QFrame* principal_frame = new QFrame(); //Et encore un conteneur pour la route
    principal_frame->setFrameStyle(QFrame::NoFrame);
    principal_frame->setLineWidth(0);
    principal_frame->setFrameStyle(QFrame::Box);
    principal_frame->setLayout(principal_frame_lay);

    return principal_frame;
}

/**
 * @brief GridEditor::build_chord
 * @param base_name Tonalité à rajouter à la liste des accords.
 * @return L'élément à rajouter à la liste des accords.
 *
 * Cette fonction crée un objet contenant les différentes possibilités d'accords à partir d'une tonalité donnée: mineur, majeur,...
 */
QTreeWidgetItem* GridEditor::build_chord(const QString base_name) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, base_name);
    //TODO: Si je vous dis que cette partie est à optimiser avec une boucle for 1->6 et un tableau de QTreeWidgetItem, vous me croyez?
    QTreeWidgetItem* children1 = new QTreeWidgetItem();
    QTreeWidgetItem* children2 = new QTreeWidgetItem();
    QTreeWidgetItem* children3 = new QTreeWidgetItem();
    QTreeWidgetItem* children4 = new QTreeWidgetItem();
    QTreeWidgetItem* children5 = new QTreeWidgetItem();
    QTreeWidgetItem* children6 = new QTreeWidgetItem();
    children1->setText(0, base_name); //Majeur
    children2->setText(0, base_name + "m"); //Mineur
    children3->setText(0, base_name + "aug"); //Augmenté
    children4->setText(0, base_name + "dim"); //Diminué
    children5->setText(0, base_name + "sus4"); //sus4
    children6->setText(0, base_name + "sus2"); //sus2
    item->addChild(children1);
    item->addChild(children2);
    item->addChild(children3);
    item->addChild(children4);
    item->addChild(children5);
    item->addChild(children6);
    return item;
}

void GridEditor::change_state() {
    if (grid->is_selection_empty() && chord_tree->isEnabled())
        chord_tree->setEnabled(false);
    else if(!grid->is_selection_empty() && !chord_tree->isEnabled())
        chord_tree->setEnabled(true);
    if (grid->is_row_selected() && !delete_row_button->isEnabled()) {
        delete_row_button->setEnabled(true);
        copy_down_button->setEnabled(true);
    }
    else if (!grid->is_row_selected() && delete_row_button->isEnabled()) {
        delete_row_button->setEnabled(false);
        copy_down_button->setEnabled(false);
    }
}

void GridEditor::import_xml() {
    QString xgrid_file = QFileDialog::getOpenFileName(this, tr("Open chords grid"), ".", tr("xgrid Files (*.xgrid)"));
    if (xgrid_file.isNull())
        return;
    delete grid;
    grid = new ChordTableWidget(xgrid_file);
    layout->addWidget(grid, 0, 1);
    title->setText(grid->get_name());
    save_button->setEnabled(true);
    add_row_button->setEnabled(true);
    rename_button->setEnabled(true);
    connect(chord_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(change_state()));
    connect(add_row_button, SIGNAL(clicked()), grid, SLOT(insert_row()));
    connect(copy_down_button, SIGNAL(clicked()), grid, SLOT(copy_down_rows()));
    connect(delete_row_button, SIGNAL(clicked()), grid, SLOT(delete_selected_row()));
}

void GridEditor::export_xml() {
    if (grid->get_name().isEmpty()) {
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
        grid->set_name(name);
        title->setText(name);
    }
    grid->to_xml(grid->get_name() + ".xgrid");
    QMessageBox mb;
    mb.setWindowTitle(tr("Grid saved"));
    mb.setText(tr("The chords grid is saved at ./") + grid->get_name() + ".xgrid");
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

void GridEditor::new_grid() {
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
    grid = new ChordTableWidget(column + 1, 10);
    layout->addWidget(grid, 0, 1);
    title->clear();
    save_button->setEnabled(true);
    add_row_button->setEnabled(true);
    rename_button->setEnabled(true);
    connect(chord_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(change_state()));
    connect(add_row_button, SIGNAL(clicked()), grid, SLOT(insert_row()));
    connect(copy_down_button, SIGNAL(clicked()), grid, SLOT(copy_down_rows()));
    connect(delete_row_button, SIGNAL(clicked()), grid, SLOT(delete_selected_row()));
}

void GridEditor::rename() {
    bool ok;
    QString name = QInputDialog::getText(this, tr("Grid name"), tr("Name:"), QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;
    title->setText(name);
    grid->set_name(name);
}
