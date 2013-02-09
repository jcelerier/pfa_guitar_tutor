#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsPixmapItem>

class myItem : public QGraphicsPixmapItem
{
public:
    myItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
};

#endif // MYITEM_H
