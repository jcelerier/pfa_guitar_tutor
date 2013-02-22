#include "Bridge.h"

/* Classe temporaire faisant le lien entre le player et la librairie
 *
 */

QTime* Bridge::globalClock = new QTime();

Bridge::Bridge():
    chordListo("G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H G D B F F G H")
{

    for(int i=0; i<10; i++) {
        chordListb.append("A");
    }

    PlayerChord* tempChord;

    for(int i=0; i<10; i++) {
        tempChord = new PlayerChord();
        tempChord->name = "A";
        tempChord->time = i * 1000;
        chordList.append(*tempChord);
    }

}

int Bridge::getElapsed() {
    return globalClock->elapsed();
}
void Bridge::startClock() {
    return globalClock->start();
}
