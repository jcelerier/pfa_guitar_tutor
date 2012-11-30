#include "MainWindow.hpp"
#include "getfilename.hpp"
#include "MainWidget.hpp"
#include <QWidget>
#include <QMenuBar>
#include <QComboBox>
#include "SongSelection.hpp"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	buildGUI();
}

void
MainWindow::createMenus()
{
	menuFichier = menuBar()->addMenu("&Fichier");

	menuOptions = menuBar()->addMenu("&Edition");
	QAction *openAudioConfiguration = menuOptions->addAction("Options Audio");
	QAction *openSongSelection = menuFichier->addAction("&Ouvrir...");
	QAction *actionQuitter = menuFichier->addAction("&Quitter");

	connect(openSongSelection, SIGNAL(triggered()), songSelectionPanel, SLOT(swapShowDialogModeless()));
	connect(openAudioConfiguration, SIGNAL(triggered()), audioConfPanel, SLOT(swapShowDialogModeless()));
	connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void
MainWindow::createToolBars()
{

}

void
MainWindow::createDockWindows()
{

}


void
MainWindow::buildGUI()
{
	QWidget * widLayout = new QWidget;

	QGridLayout *topLayout = new QGridLayout;


	// Set background colour to black
	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::black);
	setPalette(p);

	audioConfPanel = new AudioConfiguration(m_conf);
	audioConfPanel->init();

			songSelectionPanel = new SongSelection(m_conf);
	songSelectionPanel->init();

	setWindowTitle(tr("IGuitar"));
	setMinimumSize(800, 600);

	createMenus();
	createToolBars();
	createDockWindows();

	topLayout->addWidget(&m_mainWidget, 0, 0);
//	ssong.showView(topLayout);
//	topLayout->addWidget(fileSystemView, 0, 0);
	widLayout->setLayout(topLayout);
	setCentralWidget(widLayout);


}



