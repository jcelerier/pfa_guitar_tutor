#include "myitem.h"
#include "config.h"

/* Classe englobante pour les objets, afin qu'ils soient mis a l'echelle (verifier si c'est effectif)
 */

myItem::myItem(const QPixmap & pixmap, QGraphicsItem * parent):
    QGraphicsPixmapItem(pixmap, parent)
{
    //setScale(Config::quotient);
    setCursor(Qt::PointingHandCursor);
    setToolTip("Je suis un item");
}

void myItem::mousePressEvent(QGraphicsSceneMouseEvent*e)
{
    qDebug() << "item touched";
    e->accept();
}
