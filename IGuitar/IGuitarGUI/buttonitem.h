#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsPixmapItem>
#include <QtGui>

class ButtonItem : public QGraphicsObject
{
Q_OBJECT
public:
    ButtonItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
signals:
    void pushed();
private:
    QGraphicsPixmapItem* image;
};

#endif // MYITEM_H
