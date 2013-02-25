#include <QApplication>
#include "Controler.hpp"



int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  QRect screenGeo = QApplication::desktop()->screenGeometry();

  Configuration::setWindowSize(screenGeo); // Ne fonctionne pas sur les multi ecran

  new Controler();

  return app.exec();
}
