#ifndef PLAYERCHORD_H
#define PLAYERCHORD_H

#include <QGraphicsItem>

class PlayerChord
{
public:
    QString getName() const;
    int getTime() const;
    void setName(const QString n);
    void setTime(const int t);
    QGraphicsRectItem* getFullSongItem() const;
    void setFullSongItem(QGraphicsRectItem *r);
    void setScrollingChordItem(QGraphicsTextItem *r);
private:
    QString name;
    int time;
    QGraphicsRectItem* fullSongItem;
    QGraphicsTextItem* scrollingChordItem;

};

#endif // PLAYERCHORD_H
