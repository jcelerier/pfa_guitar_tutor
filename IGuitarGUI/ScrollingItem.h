#ifndef SCROLLINGITEM_H
#define SCROLLINGITEM_H

#include <QGraphicsItem>
#include "Controler.hpp"
#include <QGraphicsOpacityEffect>
#include "PlayerScene.h"
#include "Util.hpp"

class Controler;
/**
 * @brief Contient les accords defilants
 *
 * Cette classe est rafraichie a chaque rafraichissement d'ecran, determine dans la configuration
 */
class ScrollingItem : public QGraphicsItem
{
public:
    ScrollingItem(QGraphicsItem *parent = 0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget* widget= 0);
    virtual void advance ( int );
    void load(LogicalTrack*);
    void setCurrentChord(TrackChord* tc);

private:
    QPointF const m_initialPos;     // position initiale des accords
    float const m_pixPerMsec;       // Vitesse (et espace) de defilement des accords

    Controler* m_controler;
    LogicalTrack* m_track;
    bool m_loaded;

    QGraphicsItemGroup* m_scrollingTextContainer;   // Contient tous les accords pour les deplacer d'un coup
};

#endif // SCROLLINGITEM_H
