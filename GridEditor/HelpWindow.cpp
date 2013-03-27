#include "HelpWindow.h"
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
	m_page = new QWebView();
	m_page->load(QUrl("qrc:/help/help.html"));

	m_close = new QPushButton(tr("Close"));

	m_layout = new QVBoxLayout();
	m_layout->addWidget(m_page);
	m_layout->addWidget(m_close);

	setMinimumWidth(1100);
	setMaximumWidth(1100);

	setLayout(m_layout);

	connect(m_close, SIGNAL(clicked()), this, SLOT(accept()));
}

/**
 * @brief HelpWindow::~HelpWindow
 *
 * Destructeur.
 */
HelpWindow::~HelpWindow()
{
	delete m_layout;
	delete m_page;
	delete m_close;
}
