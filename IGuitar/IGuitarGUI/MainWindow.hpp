#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "audioconfiguration.h"
#include "MainWidget.hpp"
#include "SongSelection.hpp"

class MainWindow : public QMainWindow
{
		Q_OBJECT
	public:
		explicit MainWindow(QWidget *parent = 0);
		void buildGUI();
	signals:
		
	public slots:

	private:
		void createMenus();
		void createToolBars();
		void createDockWindows();
		QMenu *menuFichier;
		QMenu *menuOptions;
		AudioConfiguration *audioConfPanel;
		SongSelection *songSelectionPanel;
		MainWidget m_mainWidget;
		
};

#endif // MAINWINDOW_H
