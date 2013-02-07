#include "HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent)
{
    page = new QWebView();
    page->load(QUrl("data/help.html"));

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
