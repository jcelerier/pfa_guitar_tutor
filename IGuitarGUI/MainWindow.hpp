#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "audioconfiguration.h"
#include "MainWidget.hpp"
#include "Configuration.h"
#include "SongSelection.hpp"

class MainWindow : public QMainWindow
{
		Q_OBJECT
    friend class MainWidget;
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
        MainWidget *m_mainWidget;

        Configuration m_conf;
		
};

#endif // MAINWINDOW_H
