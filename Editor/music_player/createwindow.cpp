#include "createwindow.h"


CreateWindow::CreateWindow()
{
    this->setWindowTitle("Creer un morceau");
    this->setFixedSize(800,600);
    this->player = new Player();

    /*Connect buttons to slot*/
    QObject::connect(player->getOpenButton(),SIGNAL(clicked()),player,SLOT(open()));
    QObject::connect(player->getPlayButton(),SIGNAL(clicked()),player,SLOT(play()));
    QObject::connect(player->getPauseButton(),SIGNAL(clicked()),player,SLOT(pause()));
    QObject::connect(player->getSlider(), SIGNAL(sliderMoved(int)), player, SLOT(refreshMusic(int)));
    QObject::connect(player->getTimer(),SIGNAL(timeout()),player,SLOT(refreshTimer()));
    QObject::connect(player->getSlider(), SIGNAL(sliderMoved(int)), player, SLOT(changePosStream(int)));
    QObject::connect(player->getExportAction(),SIGNAL(triggered()),this,SLOT(exportDataToFile()));
    QObject::connect(player->getOpenAction(),SIGNAL(triggered()),player,SLOT(open()));
    QObject::connect(player->getVisualisationButton(),SIGNAL(clicked()),player,SLOT(visualiser()));

    layout = new QGridLayout(this);
    layout->addWidget(player);
    //chordTable = new ChordTableWidget();
    //layout->addWidget(chordTable);
    this->setLayout(layout);
}

void CreateWindow::exportDataToFile(){
    char *son = new char [player->nom_fichier.toStdString().size() + 1];
    strcpy (son,((player->nom_fichier.toStdString()).substr(0,player->nom_fichier.toStdString().size()-4)).c_str());
    std::ofstream fichier(son,std::ios::out | std::ios::trunc);
    for(int i = 0;i<player->tempo.size();i++)
        fichier << player->tempo[i].duration << std::endl;
    std::cout << "Data exported" << std::endl;
    fichier.close();
}

