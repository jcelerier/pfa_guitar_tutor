#include "MainWindow.hpp"
#include "MainWidget.hpp"
#include <QWidget>
#include <QMenuBar>
#include <QComboBox>
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
	QAction *actionQuitter = menuFichier->addAction("&Quitter");
	menuOptions = menuBar()->addMenu("&Edition");
	QAction *openAudioConfiguration = menuOptions->addAction("Options Audio");

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

	audioConfPanel = new AudioConfiguration;
	audioConfPanel->init();

	setWindowTitle(tr("IGuitar"));
	setMinimumSize(800, 600);

	createMenus();
	createToolBars();
	createDockWindows();

	topLayout->addWidget(&m_mainWidget, 0, 0);
	widLayout->setLayout(topLayout);
	setCentralWidget(widLayout);


}



