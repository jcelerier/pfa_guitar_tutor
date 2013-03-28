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

	QTranslator translator_our, translator_qt;
	translator_our.load(":/translations/GridEditor_fr");
	translator_qt.load(":/translations/qt_fr.qm");
	app.installTranslator(&translator_our);
	app.installTranslator(&translator_qt);

	GridEditor win;
	win.show();
	return app.exec();
}
