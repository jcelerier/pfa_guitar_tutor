#include <QApplication>
#include "Controler.hpp"



int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  new Controler();

  return app.exec();
}
