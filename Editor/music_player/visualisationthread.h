#ifndef VISUALISATIONTHREAD_H
#define VISUALISATIONTHREAD_H

#include<QThread>
#include"player.h"

class VisualisationThread : public QThread
{
    Q_OBJECT

private:
    Player *player;
    bool ended;
    int tempoPosition;
public:
    VisualisationThread(Player*);
    void run();
public slots:
    void endThread();
    void refreshMusic(int);
signals:
    void ledMustBeRed();
    void ledMustBeGreen();
};

#endif // VISUALISATIONTHREAD_H
