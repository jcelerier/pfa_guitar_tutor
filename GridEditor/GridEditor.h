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
#include "UndoManager.h"
#include "HelpWindow.h"

/**
 * @brief Fenêtre principale de l'éditeur
 */
class GridEditor : public QMainWindow
{
		Q_OBJECT
		friend class EditionSelector;
		friend class UndoManager;

		int m_barsize;
		QSettings* m_settings;

		AudioWindow* m_audioWindow;
		NewGridDialog* m_newGridDialog;

		QWidget* m_centralArea;
		QGridLayout* m_layout;
		ChordTableWidget* m_grid;
		ChordTree* m_chordTree;

		QMenu *m_fileMenu, *m_editMenu, *m_aboutMenu;

		QToolBar *m_toolBar;
		QAction *quitAction, *aboutAction, *newAction, *saveAction, *helpAction,
		*openAction, *addRowAction, *deleteRowAction, *saveAsAction,
		*copyDownAction, *addColumnAction,
		*deleteColumnAction, *openAudioWindowAction, *openTrackPropertiesAction,
		*undoAction, *redoAction;
		EditionSelector *m_editionSelector;
		EditorPanel *m_editorPanel;

		QStatusBar* m_status;
		QLabel* m_statusInfo;

		bool m_isPanelSet;

		QString m_filename;
		HelpWindow *m_helpWindow;
		TrackProperties* m_trackProperties;

		UndoManager* m_saveQueue;

	public:
		GridEditor();
		~GridEditor();
		QString statusText();
		void enableUndo(bool );
		void enableRedo(bool );
		void startGrid(int);
		void createGrid(int , int );

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
		void toXML(QString m_filename = "");
		void fromXML();
		void about();
		void setStatusText();
		void help();
};

#endif // GRIDEDITOR_H
