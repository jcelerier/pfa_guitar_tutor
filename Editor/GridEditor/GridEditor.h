/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QWidget>
#include <iostream>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QTreeWidget>
#include <QFrame>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "ChordTableWidget.h"
#include "CaseItem.h"

class GridEditor : public QWidget
{
    Q_OBJECT

    QGridLayout* layout;
    ChordTableWidget* grid;
    QTreeWidget* chord_tree;
    QLabel* title;
    //QFrame* frame;
    QPushButton* new_button;
    QPushButton* open_button;
    QPushButton* export_button;
    QPushButton* add_row_button;
    QPushButton* copy_down_button;
    QPushButton* delete_row_button;
    QPushButton* rename_button;

public:
    GridEditor();
    ~GridEditor();

private:
    QFrame* build_frame();
    void build_chord_tree();
    QTreeWidgetItem* build_chord(const QString base_name);

signals:

public slots:
    void change_state();
    void import_xml();
    void export_xml();
    void new_grid();
    void rename();
};

#endif // GRIDEDITOR_H
