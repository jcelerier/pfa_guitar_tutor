#ifndef ENTIRESONG_H
#define ENTIRESONG_H

#include <QGraphicsItem>
#include "Controler.hpp"
#include <QGraphicsOpacityEffect>
#include "PlayerScene.h"
#include "Util.hpp"

class Controler;

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
    QPointF const m_initialPos;
    float const m_pixPerMsec;

    Controler* m_controler;
    LogicalTrack* m_track;
    bool m_loaded;

    QGraphicsItemGroup* m_scrollingTextContainer;
};

#endif // ENTIRESONG_H
