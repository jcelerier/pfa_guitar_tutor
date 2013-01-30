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
#include "NewGridDialog.h"
#include "Track/TrackLoader.h"

/**
 * @brief Fenêtre principale de l'éditeur
 */
class GridEditor : public QMainWindow
{
    Q_OBJECT

    QSettings* settings;

	AudioWindow* audioWindow;
	TrackProperties* trackProperties;
	NewGridDialog* newGridDialog;

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

	QStatusBar* status;
	QLabel* statusInfo;

    bool isGridSet;

public:
    GridEditor();
    ~GridEditor();
    void newEditor(int);
	QString statusText();

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
    void play(int);

public slots:
	void changeState();
	void newGrid();
	void toXML();
	void fromXML();
    void about();
	void setStatusText();
    void rename();
};

#endif // GRIDEDITOR_H
