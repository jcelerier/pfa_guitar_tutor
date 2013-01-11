/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QCoreApplication>
#include <QtGui>
#include "ChordTableWidget.h"
#include "CaseItem.h"
#include "EditionSelector.h"
#include "ChordTree.h"
#include "SimpleMusicPlayer.h"
#include "AudioSync.h"
#include "AudioWindow.h"
#include "TrackProperties.h"

/**
 * @brief Fenêtre principale de l'éditeur
 */
class GridEditor : public QMainWindow
{
    Q_OBJECT

	AudioWindow* audioWindow;
	TrackProperties* trackProperties;

    QWidget* centralArea;
    QGridLayout* layout;
    ChordTableWidget* grid;
    ChordTree* chordTree;

    QMenu *fileMenu, *editMenu, *optionMenu, *aboutMenu;

    QToolBar *toolBar;
    QAction *quitAction, *aboutAction, *newAction, *saveAction,
            *openAction, *addRowAction, *deleteRowAction,
            *copyDownAction, *renameAction, *addColumnAction,
			*deleteColumnAction, *openAudioWindowAction, *openTrackPropertiesAction;
    EditionSelector *editionSelector;

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
    QTreeWidgetItem* buildChord(const QString tonality);

signals:
	void sendTimeToChordWidget(QTime, QTime, QTime);

public slots:
	void changeState();
	void newGrid();
    void newEditor(int);

	void openAudioWindow();
	void openTrackProperties();

	void toXML();
};

#endif // GRIDEDITOR_H
