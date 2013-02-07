#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QtGui>

class HelpWindow : public QDialog
{
    Q_OBJECT

    QTextEdit *text;
    QPushButton *close;
    QVBoxLayout *layout;
public:
    explicit HelpWindow(QWidget *parent = 0);
};

#endif // HELPWINDOW_H
