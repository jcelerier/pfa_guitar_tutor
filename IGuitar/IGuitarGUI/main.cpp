#include <QApplication>

#include "MainWindow.hpp"
#include "GuitarWindow.h"
#include "selectsong.hpp"

int main(int argc, char *argv[]){
  QApplication app(argc, argv);
 // MainWindow* mainWindow = new MainWindow();
  //mainWindow->show();
  //GuitarWindow* gWindow = new GuitarWindow();
  //gWindow->show();
  QString path = "./Tracks/BeatlesDayInTheLife/";
  SelectSong* sSong = new SelectSong(path);
  sSong->show();
  return app.exec();
}
