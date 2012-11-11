/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 28/02/12
*/

#include <QApplication>
#include "GridEditor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GridEditor win;
    win.show();
    return app.exec();
}
