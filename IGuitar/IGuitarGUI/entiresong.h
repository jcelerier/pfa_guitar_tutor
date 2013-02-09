#ifndef ENTIRESONG_H
#define ENTIRESONG_H

#include <QGraphicsItem>

class EntireSong : public QGraphicsItem
{
public:
    EntireSong(QList<QString> chordlist, QGraphicsItem *parent = 0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void nextChord();
    virtual void advance ( int );
signals:
    
public slots:
private:
    QList<QGraphicsRectItem*> cList;
    int currentChord;

    
};

#endif // ENTIRESONG_H
