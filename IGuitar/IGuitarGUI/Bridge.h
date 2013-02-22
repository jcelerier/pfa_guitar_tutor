#ifndef BRIDGE_H
#define BRIDGE_H

#include <QtGui>
#include "playerchord.h"
class Bridge
{
private:

public:
    Bridge();
    QString chordListo;
    QList<QString> chordListb;
    QList<PlayerChord> chordList;

    static QTime* globalClock;
    static int getElapsed();
    static void startClock();
};

#endif // BRIDGE_H
