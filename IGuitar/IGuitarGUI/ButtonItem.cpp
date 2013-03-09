#include "ButtonItem.h"

/* Classe englobante pour les objets, afin qu'ils soient mis a l'echelle (verifier si c'est effectif)
 */

/**
 * @brief ButtonItem::ButtonItem
 * @param pixmap Image
 * @param parent Element parent
 *
 * Construit un bouton avec l'image donnée sur le parent.
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

/**
 * @brief ButtonItem::mousePressEvent
 * @param e Evènement
 *
 * Méthode appelée lors d'un clic souris sur un bouton.
 */
void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    qDebug() << "button touched";
    emit pushed();
}

/**
 * @brief ButtonItem::paint
 *
 * Non utilisé, mais nécessaire (méthode abstraite dans QGraphicsItem).
 */
void ButtonItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){

}

/**
 * @brief ButtonItem::boundingRect
 * @return Les contours du bouton
 *
 * Retourne les contours du bouton.
 */
QRectF ButtonItem::boundingRect() const {
    return image->boundingRect();

}
