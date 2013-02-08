/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QMainWindow>
#include <QSettings>
#include <QWidget>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QString>
#include <QTime>
#include <QTreeWidgetItem>
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
#include "EditorPanel.h"
#include "HelpWindow.h"

/**
 * @brief Fenêtre principale de l'éditeur
 */
class GridEditor : public QMainWindow
{
    Q_OBJECT
friend class EditionSelector;

    QSettings* settings;

	AudioWindow* audioWindow;
	TrackProperties* trackProperties;
	NewGridDialog* newGridDialog;

    QWidget* centralArea;
    QGridLayout* layout;
    ChordTableWidget* grid;
    ChordTree* chordTree;

    QMenu *fileMenu, *editMenu, *optionMenu, *gridMenu, *aboutMenu;

    QToolBar *toolBar;
    QAction *quitAction, *aboutAction, *newAction, *saveAction, *helpAction,
            *openAction, *addRowAction, *deleteRowAction, *saveAsAction,
            *copyDownAction, *renameAction, *addColumnAction,
			*deleteColumnAction, *openAudioWindowAction, *openTrackPropertiesAction;
    EditionSelector *editionSelector;
	EditorPanel *editorPanel;

	QStatusBar* status;
	QLabel* statusInfo;

    bool isGridSet;

    QString filename;

public:
    GridEditor();
    ~GridEditor();
	QString statusText();
    void startGrid(int);
	void createGrid(int columns, int rows);
private:
    void createMenu();
    void createActions();
    void setActionsToMenu();
    void createToolbar();
    void createCentralWidget();
    void connectActionToSlot();
    void buildChordTree();
    QTreeWidgetItem* buildChord(const QString tonality);
    bool saveBeforeQuit();

signals:
	void sendTimeToChordWidget(QTime, QTime, QTime);
    void play(int);

public slots:
	void changeState();
	void firstNewGrid();
	void newGrid();
    void save();
    void toXML(QString filename = "");
	void fromXML();
    void about();
	void setStatusText();
    void rename();
    void help();
};

#endif // GRIDEDITOR_H
