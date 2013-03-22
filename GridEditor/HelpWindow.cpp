#include "HelpWindow.h"
#include <QDebug>
#include <QDir>

/**
 * @brief HelpWindow::HelpWindow
 * @param parent Widget parent
 *
 * Crée la fenetre d'aide utilisateur.
 */
HelpWindow::HelpWindow(QWidget *parent) :
	QDialog(parent)
{
	page = new QWebView();
	page->load(QUrl("qrc:/help/help.html"));

	close = new QPushButton(tr("Close"));

	layout = new QVBoxLayout();
	layout->addWidget(page);
	layout->addWidget(close);

    setMinimumWidth(1100);
    setMaximumWidth(1100);

	setLayout(layout);

	connect(close, SIGNAL(clicked()), this, SLOT(accept()));
}

/**
 * @brief HelpWindow::~HelpWindow
 *
 * Destructeur.
 */
HelpWindow::~HelpWindow()
{
	delete layout;
	delete page;
	delete close;
}
