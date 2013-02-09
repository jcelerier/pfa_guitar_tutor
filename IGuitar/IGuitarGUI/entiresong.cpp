#include "entiresong.h"

#include <QPen>
#include <QFont>

EntireSong::EntireSong(QList<QString> chordlist, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    currentChord(0)
{
    QPen borderPen(Qt::black, 3);
    QPen lightPen(Qt::black, 1);
    QBrush innerCont(Qt::white);
    QFont chordFont("Roboto", 36);

    QGraphicsRectItem* lineCont = new QGraphicsRectItem(1470, 108, 355, 53, this);
    lineCont->setBrush(innerCont);
    lineCont->setPen(borderPen);


    QList<QString>::iterator ite;
    QGraphicsRectItem* tempCase;
    QGraphicsTextItem* tempText;

    int i=0, j=0;
    for (ite = chordlist.begin(); ite != chordlist.end(); ++ite)
    {
        tempCase = new QGraphicsRectItem(1471 + 355/4*i, 108 + 73*j, 355/4, 53, this);
        tempCase->setBrush(innerCont);
        tempCase->setPen(lightPen);
        cList.append(tempCase);

        tempText = new QGraphicsTextItem(tempCase);
        tempText->setFont(chordFont);
        tempText->setDefaultTextColor(QColor(14,153,204));
        tempText->setTextWidth(355/4);
        tempText->setHtml("<p align='center'>"+*ite+"</p>");
        tempText->setPos(tempCase->rect().topLeft());
        if(i==3)
        {
            j++;
            i=0;
        }
        else
            i++;
    }

}

void EntireSong::nextChord() {
    QBrush newCol(Qt::green);
    cList[currentChord]->setBrush(newCol);
    currentChord++;
    if(currentChord>=cList.size())
        currentChord=0;
}

void EntireSong::advance ( int phase ) {
    if(phase == 1)
        nextChord();
}

void EntireSong::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

}

QRectF EntireSong::boundingRect() const {
    return QRectF(0,0,1920,1080);

}
