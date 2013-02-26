#include <QtWidgets/QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[]){
  QApplication app(argc, argv);
  QString path = "./Tracks/";
  MainWindow* mainWindow = new MainWindow();
  mainWindow->show();

  //
  //SelectSong* sSong = new SelectSong(path);
 // sSong->show();
  return app.exec();
}
