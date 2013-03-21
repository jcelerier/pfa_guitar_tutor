#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include "Controler.hpp"

int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  QTranslator translator;
  translator.load(":/translations/IGuitarGUI_fr");
  app.installTranslator(&translator);

  QFontDatabase::addApplicationFont(":/fonts/Roboto.ttf");

  QRect screenGeo = QApplication::desktop()->screenGeometry();

  Configuration::setWindowSize(screenGeo); // Ne fonctionne pas sur les multi ecran
  Controler c; // sinon le destructeur n'est pas appelé

  return app.exec();
}
