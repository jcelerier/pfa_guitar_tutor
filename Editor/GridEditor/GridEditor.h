#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QSettings>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLabel>
#include <QString>
#include <QTime>
#include <QtWidgets/QTreeWidgetItem>
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

int m_barsize;
	QSettings* settings;

	AudioWindow* audioWindow;
	TrackProperties* trackProperties;
	NewGridDialog* newGridDialog;

	QWidget* centralArea;
	QGridLayout* layout;
	ChordTableWidget* grid;
	ChordTree* chordTree;

	QMenu *fileMenu, *editMenu, *aboutMenu;

	QToolBar *toolBar;
	QAction *quitAction, *aboutAction, *newAction, *saveAction, *helpAction,
			*openAction, *addRowAction, *deleteRowAction, *saveAsAction,
			*copyDownAction, *addColumnAction,
			*deleteColumnAction, *openAudioWindowAction, *openTrackPropertiesAction;
	EditionSelector *editionSelector;
	EditorPanel *editorPanel;

	QStatusBar* status;
	QLabel* statusInfo;

	bool isPanelSet;

	QString filename;

public:
	GridEditor();
	~GridEditor();
	QString statusText();
	void startGrid(int);
	void createGrid(int columns, int rows);

	int getBarSize();

private:
	void createMenu();
	void createActions();
	void setActionsToMenu();
	void createToolbar();
	void createCentralWidget();
	void connectActionToSlot();
	bool saveBeforeQuit();

signals:
	void sendTimeToChordWidget(QTime, QTime, QTime);
	void play(int);
	void propsChanged();
	void sigTimeData(QTime);

public slots:
	void barsizeChanged(int);
	void changeState();
	void firstNewGrid();
	void newGrid();
	void save();
	void toXML(QString filename = "");
	void fromXML();
	void about();
	void setStatusText();
	void help();
};

#endif // GRIDEDITOR_H
