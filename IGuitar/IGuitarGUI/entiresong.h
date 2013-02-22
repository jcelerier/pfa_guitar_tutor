#ifndef ENTIRESONG_H
#define ENTIRESONG_H

#include <QGraphicsItem>
#include <Bridge.h>

class EntireSong : public QGraphicsItem
{
public:
    EntireSong(QGraphicsItem *parent = 0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void nextChord();
    virtual void advance ( int );
signals:
    
public slots:
private:
    Bridge bridge;
    QList<QGraphicsRectItem*> cList;
    int currentChord;
    int time;
    QGraphicsItemGroup* scrollingTextContainer;
    QGraphicsTextItem* timeText;

    float pixPerMsec;
    
};

#endif // ENTIRESONG_H
