/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include "GridEditor.h"

GridEditor::GridEditor() : QWidget()
{
    QDesktopWidget desktop;
    this->setBaseSize(3*desktop.width()/4, 3*desktop.height()/4);
    QFrame* frame = build_frame();
    build_chord_tree();
    layout = new QGridLayout();
    grid = new ChordTableWidget();
    layout->addWidget(chord_tree, 0, 0);
    layout->addWidget(grid, 0, 1);
    layout->addWidget(frame, 0, 2);
    this->setLayout(layout);
    export_button->setEnabled(false);
    delete_row_button->setEnabled(false);
    copy_down_button->setEnabled(false);
    add_row_button->setEnabled(false);
    rename_button->setEnabled(false);
    connect(chord_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(change_state()));
    connect(export_button, SIGNAL(clicked()), this, SLOT(export_xml()));
    connect(open_button, SIGNAL(clicked()), this, SLOT(import_xml()));
    connect(add_row_button, SIGNAL(clicked()), grid, SLOT(insert_row()));
    connect(copy_down_button, SIGNAL(clicked()), grid, SLOT(copy_down_rows()));
    connect(delete_row_button, SIGNAL(clicked()), grid, SLOT(delete_selected_row()));
    connect(new_button, SIGNAL(clicked()), this, SLOT(new_grid()));
    connect(rename_button, SIGNAL(clicked()), this, SLOT(rename()));
}

GridEditor::~GridEditor()
{
    delete grid;
    delete chord_tree;
    delete open_button;
    delete export_button;
    delete add_row_button;
    delete copy_down_button;
    delete delete_row_button;
    delete layout;
}

void GridEditor::build_chord_tree()
{
    chord_tree = new QTreeWidget();
    chord_tree->setFixedWidth(200);
    chord_tree->setHeaderLabel("Chord choice");
    chord_tree->setDisabled(true);
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

QFrame* GridEditor::build_frame()
{
    QVBoxLayout* top_frame_lay = new QVBoxLayout();
    new_button = new QPushButton("New");
    top_frame_lay->addWidget(new_button);
    rename_button = new QPushButton("Rename");
    top_frame_lay->addWidget(rename_button);
    open_button = new QPushButton("Open");
    top_frame_lay->addWidget(open_button);
    export_button = new QPushButton("Save");
    top_frame_lay->addWidget(export_button);
    QVBoxLayout* bottom_frame_lay = new QVBoxLayout();
    copy_down_button = new QPushButton("Copy down");
    bottom_frame_lay->addWidget(copy_down_button);
    add_row_button = new QPushButton("Insert row");
    bottom_frame_lay->addWidget(add_row_button);
    delete_row_button = new QPushButton("Delete row");
    bottom_frame_lay->addWidget(delete_row_button);
    QVBoxLayout* frame_lay = new QVBoxLayout();
    frame_lay->addLayout(top_frame_lay);
    frame_lay->addLayout(bottom_frame_lay);
    QFrame* frame = new QFrame();
    frame->setFrameStyle(QFrame::Box);
    frame->setLayout(frame_lay);
    QVBoxLayout* principal_frame_lay = new QVBoxLayout();
    title = new QLabel();
    title->setFixedHeight(30);
    title->setAlignment(Qt::AlignHCenter);
    QFont title_font;
    title_font.setPointSize(14);
    title_font.setBold(true);
    title->setFont(title_font);
    principal_frame_lay->addWidget(title);
    principal_frame_lay->addWidget(frame);
    QFrame* principal_frame = new QFrame();
    principal_frame->setFrameStyle(QFrame::NoFrame);
    principal_frame->setLineWidth(0);
    principal_frame->setFrameStyle(QFrame::Box);
    principal_frame->setLayout(principal_frame_lay);
    return principal_frame;
}

QTreeWidgetItem* GridEditor::build_chord(const QString base_name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, base_name);
    QTreeWidgetItem* children1 = new QTreeWidgetItem();
    QTreeWidgetItem* children2 = new QTreeWidgetItem();
    QTreeWidgetItem* children3 = new QTreeWidgetItem();
    QTreeWidgetItem* children4 = new QTreeWidgetItem();
    QTreeWidgetItem* children5 = new QTreeWidgetItem();
    QTreeWidgetItem* children6 = new QTreeWidgetItem();
    children1->setText(0, base_name);
    children2->setText(0, base_name + "m");
    children3->setText(0, base_name + "aug");
    children4->setText(0, base_name + "dim");
    children5->setText(0, base_name + "sus4");
    children6->setText(0, base_name + "sus2");
    item->addChild(children1);
    item->addChild(children2);
    item->addChild(children3);
    item->addChild(children4);
    item->addChild(children5);
    item->addChild(children6);
    return item;
}

void GridEditor::change_state()
{
    if (grid->is_selection_empty() && chord_tree->isEnabled())
        chord_tree->setEnabled(false);
    else if(!grid->is_selection_empty() && !chord_tree->isEnabled())
        chord_tree->setEnabled(true);
    if (grid->is_row_selected() && !delete_row_button->isEnabled())
    {
        delete_row_button->setEnabled(true);
        copy_down_button->setEnabled(true);
    }
    else if (!grid->is_row_selected() && delete_row_button->isEnabled())
    {
        delete_row_button->setEnabled(false);
        copy_down_button->setEnabled(false);
    }
}

void GridEditor::import_xml()
{
    QString xgrid_file = QFileDialog::getOpenFileName(this, tr("Open chords grid"), ".", tr("xgrid Files (*.xgrid)"));
    if (xgrid_file.isNull())
        return;
    delete grid;
    grid = new ChordTableWidget(xgrid_file);
    layout->addWidget(grid, 0, 1);
    title->setText(grid->get_name());
    export_button->setEnabled(true);
    add_row_button->setEnabled(true);
    rename_button->setEnabled(true);
    connect(chord_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(change_state()));
    connect(add_row_button, SIGNAL(clicked()), grid, SLOT(insert_row()));
    connect(copy_down_button, SIGNAL(clicked()), grid, SLOT(copy_down_rows()));
    connect(delete_row_button, SIGNAL(clicked()), grid, SLOT(delete_selected_row()));
}

void GridEditor::export_xml()
{
    if (grid->get_name().isEmpty())
    {
        bool ok;
        QString name;
        do
        {
            name = QInputDialog::getText(this, "Grid name", "Name the current grid to save it.\n\n Name:", QLineEdit::Normal, "", &ok);
            if (!ok)
                return;
            if (name.isEmpty())
            {
                QMessageBox mb;
                mb.setWindowTitle("Warning");
                mb.setText("You have to enter a name to continue.");
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
    mb.setWindowTitle("Grid saved");
    mb.setText("The chords grid is saved at ./" + grid->get_name() + ".xgrid");
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

void GridEditor::new_grid()
{
    bool ok;
    QString mess;
    if (grid->rowCount() == 0)
        mess = "Column number:";
    else
        mess = "Warning:\nUnsaved modifications on current grid will be lost.\n\nColumn number:";
    int column = QInputDialog::getInt(this, tr("New grid"), tr(mess.toAscii()), 4, 1, 64, 1, &ok);
    if (!ok)
        return;
    delete grid;
    grid = new ChordTableWidget(column + 1, 10);
    layout->addWidget(grid, 0, 1);
    title->clear();
    export_button->setEnabled(true);
    add_row_button->setEnabled(true);
    rename_button->setEnabled(true);
    connect(chord_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), grid, SLOT(fill_selection(QTreeWidgetItem*,int)));
    connect(grid, SIGNAL(itemSelectionChanged()), this, SLOT(change_state()));
    connect(add_row_button, SIGNAL(clicked()), grid, SLOT(insert_row()));
    connect(copy_down_button, SIGNAL(clicked()), grid, SLOT(copy_down_rows()));
    connect(delete_row_button, SIGNAL(clicked()), grid, SLOT(delete_selected_row()));
}

void GridEditor::rename()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Grid name", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;
    title->setText(name);
    grid->set_name(name);
}
