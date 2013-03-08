#ifndef ENTIRESONG_H
#define ENTIRESONG_H

#include <QGraphicsItem>

class Controler;

class EntireSong : public QGraphicsItem
{
public:
    EntireSong(QGraphicsItem *parent = 0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void nextChord();
    virtual void advance ( int );
    void validateChord(bool v);
    QString getCurrentChord() const;
    int getCurrentDuration() const;
    int getTotalPlayedChords() const;
    int getTotalValidatedChords() const;
    bool getIsCurrentChordValidated() const;
private:
    QList<QGraphicsRectItem*> cList;
    int currentChord;
    int lastRefresh;
    int totalPlayedChords;
    int totalValidatedChords;
    QGraphicsItemGroup* scrollingTextContainer;
    QGraphicsTextItem* timeText;

    float pixPerMsec;

    Controler* controler;
    bool isCurrentChordValidated;
    
};

#endif // ENTIRESONG_H
