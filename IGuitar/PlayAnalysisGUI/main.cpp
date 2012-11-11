#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[]){
  QApplication app(argc, argv);
  MainWindow* mainWindow = new MainWindow();
  mainWindow->setArgs(argc, argv);
  mainWindow->show();
  return app.exec();
}
