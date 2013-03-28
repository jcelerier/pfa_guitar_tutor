#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QLabel>
#include <QSettings>
#include <QTime>

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

	public:
		GridEditor();
		~GridEditor();
		QString statusText();
		void enableUndo(bool);
		void enableRedo(bool);
		void startGrid(int);
		void createGrid(int, int);

		void closeEvent(QCloseEvent *event);

	signals:
		/**
		 * @brief sendTimeToChordWidget Transfère les informations de temps d'audioWindow vers la grille
		 */
		void sendTimeToChordWidget(QTime, QTime, QTime);

		/**
		 * @brief play est utilisé pour la lecture
		 */
		void play(int);

		/**
		 * @brief propsChanged est utilisé pour mettre à jour le texte de la status bar
		 */
		void propsChanged();

		/**
		 * @brief sigTimeData est utilisé pour dire à la grille là ou on en est (pour l'affichage de la lecture en temps réel)
		 */
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

	private:
		void createMenu();
		void createActions();
		void setActionsToMenu();
		void createToolbar();
		void createCentralWidget();
		void connectActionToSlot();
		bool saveBeforeQuit();

		/**
		 * @brief m_barsize Taille de la mesure
		 */
		int m_barsize;

		/**
		 * @brief m_settings Propriétés du programme (pour sauvegarde au redémarrage)
		 */
		QSettings* m_settings;

		/**
		 * @brief m_audioWindow cf. AudioWindow
		 */
		AudioWindow* m_audioWindow;

		/**
		 * @brief m_newGridDialog cf. NewGridDialog
		 */
		NewGridDialog* m_newGridDialog;

		/**
		 * @brief m_trackProperties cf. TrackProperties
		 */
		TrackProperties* m_trackProperties;

		/**
		 * @brief m_saveQueue cf. UndoManager
		 */
		UndoManager* m_saveQueue;

		/**
		 * @brief m_centralArea Zone centrale ou on dessine tout
		 */
		QWidget* m_centralArea;

		/**
		 * @brief m_layout Organisation de m_centralArea
		 */
		QGridLayout* m_layout;

		/**
		 * @brief m_grid Grille d'accords
		 */
		ChordTableWidget* m_grid;

		/**
		 * @brief m_chordTree Arbre d'accords sur la gauche
		 */
		ChordTree* m_chordTree;

		/**
		 * @brief m_editionSelector cf. EditionSelector
		 */
		EditionSelector *m_editionSelector;

		/**
		 * @brief m_editorPanel cf. EditorPanel
		 */
		EditorPanel *m_editorPanel;

		/**
		 * @brief m_helpWindow cf. HelpWindow
		 */
		HelpWindow *m_helpWindow;

		/**
		 * @brief m_status Barre de status (affichage : artiste / piste, en bas)
		 */
		QStatusBar* m_status;

		/**
		 * @brief m_statusInfo Label ou on affiche les informations de m_status
		 */
		QLabel* m_statusInfo;

		/**
		 * @brief m_isPanelSet Vrai si un morceau est chargé
		 */
		bool m_isPanelSet;

		QString m_filename;

		QMenu *m_fileMenu, *m_editMenu, *m_aboutMenu;

		QToolBar *m_toolBar;

		QAction *quitAction, *aboutAction, *newAction, *saveAction, *helpAction,
		*openAction, *addRowAction, *deleteRowAction, *saveAsAction,
		*copyDownAction, *addColumnAction,
		*deleteColumnAction, *openAudioWindowAction, *openTrackPropertiesAction,
		*undoAction, *redoAction;

};

#endif // GRIDEDITOR_H
