//#include <stdio.h>
//#include <stdlib.h>
//#include <map>
//#include <string>
//
//#include "MusicManager.h"
//#include "ScoreManager.h"
//
//#include <QApplication>
//
//#include "gui/MainWindow.hpp"
//
//
//
////int main(void)
////{
////	std::map<std::string, std::string> multiTracksMap;
//////	multiTracksMap["guitar"] = "Tracks/MuseHysteria/Guitar.wav";
//////	multiTracksMap["bass"] = "Tracks/MuseHysteria/Bass.wav";
//////	multiTracksMap["background"] = "Tracks/MuseHysteria/Background.wav";
////	multiTracksMap["all"] = "Tracks/BeatlesDayInTheLife/AllTracks.wav";
////
////
////	MusicManager* musicManager = new MusicManager(multiTracksMap);
////	ScoreManager* scoreManager = new ScoreManager(musicManager);
////
////	scoreManager->loadScore("Tracks/BeatlesDayInTheLife/Guitar.txt");
////	scoreManager->run();
////
////	usleep(100000);
////	scoreManager->setNextPart("[VERSE1]");
////
////	while (scoreManager->isRunning()) {
////
////	//	musicManager->goToInMs(20000);
////		usleep(200000);
////		scoreManager->update();
//////		std::string input;
//////		std::cout << "next part ?" << std::endl;
//////		std::cin >> input;
//////
////		//scoreManager->setNextPart(input);
////		//std::cout << "the next part will be: " << input << std::endl;
////		std::cout << "and current chord is " << scoreManager->getCurrentChord() << std::endl;
////	}
////
////	delete scoreManager;
////	delete musicManager;
//
////}
//
//int main(int argc, char *argv[]){
//  QApplication app(argc, argv);
//  MainWindow* mainWindow = new MainWindow();
//  mainWindow->show();
//  return app.exec();
//}
//
//

