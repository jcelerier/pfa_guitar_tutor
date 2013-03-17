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
    QString m_name;
    int m_time;
    QGraphicsRectItem* m_fullSongItem;
    QGraphicsTextItem* m_scrollingChordItem;

};

#endif // PLAYERCHORD_H
