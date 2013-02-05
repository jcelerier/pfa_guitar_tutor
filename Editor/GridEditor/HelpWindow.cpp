#include "HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent)
{
    QTextEdit* text = new QTextEdit();
    text->setHtml(QString("L'avantage de <strong>cettte</strong> fenêtre, c'est qu'on peut y mettre un peu ce qu'on veut...<br />")+
                  QString("y compris du <a href=\"w3c.org\">HTML</a> et des images"));	//Mode HTML
    text->setReadOnly(true);	//Mode lecture seule
    text->setTextInteractionFlags(Qt::TextBrowserInteraction);
    text->setFont(QFont("Courier"));

    QPushButton *close = new QPushButton(tr("Close"));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(text);
    layout->addWidget(close);

    setLayout(layout);

    connect(close, SIGNAL(clicked()), this, SLOT(accept()));
}
