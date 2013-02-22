/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 28/02/12
*/

#include <QApplication>
#include "GridEditor.h"

/**
 * @brief main
 * @param argc
 * @param argv
 * @return Le résultat de l'exécution de l'éditeur.
 *
 * Fonction main.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("GridEditor_fr");
    app.installTranslator(&translator);

    GridEditor win;
    win.show();
    return app.exec();
}
