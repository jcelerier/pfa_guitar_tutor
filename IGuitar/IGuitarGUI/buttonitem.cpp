#include "buttonitem.h"
#include "config.h"

/* Classe englobante pour les objets, afin qu'ils soient mis a l'echelle (verifier si c'est effectif)
 */

ButtonItem::ButtonItem(const QPixmap & pixmap, QGraphicsItem * parent):
   // QGraphicsPixmapItem(pixmap, parent)
    QGraphicsObject(parent)
{
    //setScale(Config::quotient);
    setCursor(Qt::PointingHandCursor);
    setToolTip("Je suis un item");
    image = new QGraphicsPixmapItem(pixmap, this);
    ((QGraphicsPixmapItem*) image)->setTransformationMode(Qt::SmoothTransformation);
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent*e)
{
    qDebug() << "button touched";
    emit pushed();
}

void ButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

}

QRectF ButtonItem::boundingRect() const {
    return image->boundingRect();

}
