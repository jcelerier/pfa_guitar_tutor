#ifndef PLAYERCHORD_H
#define PLAYERCHORD_H

#include <QGraphicsItem>

class PlayerChord
{
public:
    PlayerChord();
    QString getName();
    QString name;
    int time;
    QGraphicsRectItem* fullSongItem;
    QGraphicsTextItem* scrollingChordItem;
};

#endif // PLAYERCHORD_H
