#ifndef PLAYERCHORD_H
#define PLAYERCHORD_H

#include <QGraphicsItem>

class PlayerChord
{
public:
    PlayerChord();
    QString getName() const;
    int getTime() const;
    QGraphicsRectItem* fullSongItem;
    QGraphicsTextItem* scrollingChordItem;
    void setName(const QString n);
    void setTime(const int t);
private:
    QString name;
    int time;

};

#endif // PLAYERCHORD_H
