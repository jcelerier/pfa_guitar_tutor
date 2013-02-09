#include <QApplication>
#include "MainWidget.hpp"



int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  new MainWidget();

  return app.exec();
}
