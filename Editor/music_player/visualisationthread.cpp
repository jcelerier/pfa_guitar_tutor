#include "visualisationthread.h"


VisualisationThread::VisualisationThread(Player * p) : QThread(){
    player = p;
    start();
}


void VisualisationThread::run()
{
    std::cout << "thread started" << std::endl;
    unsigned int position, oldposition, redDuration;
    oldposition = 0;
    ended = false;
    tempoPosition = 0;
    while(!ended){
        player->channel->getPosition(&position, FMOD_TIMEUNIT_MS);
        if(oldposition != position){
        oldposition = position;
        std::cout << "current position"<< position << std::endl;
}
        if ( (position < (player->tempo[tempoPosition].position+30)) && (position > (player->tempo[tempoPosition].position-30)) ){
            std::cout << "un temps ici : " << player->tempo[tempoPosition].position << std::endl;
            emit ledMustBeRed();
            if(tempoPosition < (player->tempo.size() - 1))
                redDuration = player->tempo[tempoPosition+1].duration/2;
            else
                redDuration = 60;
            if(redDuration >= 60)
                msleep(60);
            else
                msleep(player->tempo[tempoPosition+1].duration/2);
            emit ledMustBeGreen();
            tempoPosition++;
        }
    }
    emit terminated();
}


void VisualisationThread::endThread(){
    ended = true;
}

void VisualisationThread::refreshMusic(int newPosition){
    int i = 0;
    while(player->tempo[i].position < newPosition)
        i++;
    tempoPosition = i;
    std::cout << "je refresh visu" << std::endl;
}

