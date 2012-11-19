/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QMainWindow>
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
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include "ChordTableWidget.h"
#include "CaseItem.h"

class GridEditor : public QMainWindow
{
    Q_OBJECT

    QWidget* centralArea;
    QGridLayout* layout;
    ChordTableWidget* grid;
    QTreeWidget* chordTree;
    QLabel* title;

    QMenu *fileMenu, *editMenu, *optionMenu, *aboutMenu;
    QToolBar *toolBar;
    QAction *quitAction, *aboutAction, *newAction, *saveAction,
            *openAction, *addRowAction, *deleteRowAction,
            *copyDownAction, *renameAction;

public:
    GridEditor();
    ~GridEditor();

private:
    void createMenu();
    void createActions();
    void setActionsToMenu();
    void createToolbar();
    void createCentralWidget();
    void connectActionToSlot();
    void buildChordTree();
    QTreeWidgetItem* buildChord(const QString base_name);

signals:

public slots:
    void changeState();
    void importXml();
    void exportXml();
    void newGrid();
    void rename();
};

#endif // GRIDEDITOR_H
