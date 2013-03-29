#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include "Controler.hpp"

int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  QTranslator translator;
  translator.load(":/translations/IGuitarGUI_fr");
  app.installTranslator(&translator);

  QFontDatabase::addApplicationFontFromData(":/fonts/Roboto-Regular.ttf");
  QFontDatabase::addApplicationFontFromData(":/fonts/Roboto-Black.ttf");
  QFontDatabase::addApplicationFont(":/fonts/Roboto-BoldCondensed.ttf");
  QFontDatabase::addApplicationFont(":/fonts/Roboto-Italic.ttf");

  QIcon appIcon(":/images/playerIcon.png");
  QApplication::setWindowIcon(appIcon);

  QRect screenGeo = QApplication::desktop()->screenGeometry();

  Configuration::setWindowSize(screenGeo); // Ne fonctionne pas sur les multi ecran
  Controler c; // sinon le destructeur n'est pas appelé

  return app.exec();
}
