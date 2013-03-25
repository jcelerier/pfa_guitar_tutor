#ifndef ENTIRESONG_H
#define ENTIRESONG_H

#include <QGraphicsItem>
#include <Track/LogicalTrack.h>

class Controler;

class EntireSong : public QGraphicsItem
{
public:
	EntireSong(QGraphicsItem *parent = 0);
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget* widget= 0);
    void nextChord();
    void setCurrentChord(TrackChord *tc);
    void load(LogicalTrack*);

private:

    void paintChord(TrackChord*);
    TrackChord* m_currentChord;

    Controler* m_controler;

    QMap<TrackChord*, QGraphicsRectItem*> m_gMap;
    LogicalTrack* m_track;
    bool m_loaded;
    QGraphicsItemGroup* m_container;
};

#endif // ENTIRESONG_H
