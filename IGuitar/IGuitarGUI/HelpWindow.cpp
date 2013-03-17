#include "HelpWindow.h"
#include <QDebug>
#include <QDir>
HelpWindow::HelpWindow(QWidget *parent) :
	QDialog(parent)
{
    setModal(true);
	page = new QWebView();
	page->load(QUrl("qrc:/help/help.html"));

	close = new QPushButton(tr("Close"));

	layout = new QVBoxLayout();
	layout->addWidget(page);
	layout->addWidget(close);

	setLayout(layout);

	connect(close, SIGNAL(clicked()), this, SLOT(accept()));
}

HelpWindow::~HelpWindow()
{
	delete layout;
	delete page;
	delete close;
}
