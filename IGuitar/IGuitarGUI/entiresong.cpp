#include "entiresong.h"

EntireSong::EntireSong(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    currentChord(0),
    pixPerMsec(0.3)
{
    QPen borderPen(Qt::black, 3);
    QPen lightPen(Qt::black, 1);
    QBrush innerCont(Qt::white);
    QFont chordFont("Roboto", 36);
    QFont scrollingChordFont("Roboto", 60);

   /* QGraphicsRectItem* lineCont = new QGraphicsRectItem(1470, 108, 355, 53, this);
    lineCont->setBrush(innerCont);
    lineCont->setPen(borderPen);*/


    // Conteneur pour les accords défilants
    QPixmap playingMask("maskplaying.png");
    QGraphicsSimpleTextItem* maskingTextContainer = new QGraphicsSimpleTextItem(this);
    maskingTextContainer->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    maskingTextContainer->setOpacity(0.01); // Pour ne pas voir le masque mais qu'il ne soit pas desactivé

    // Masque pour cacher ce qui est hors du cadre
    QGraphicsOpacityEffect* textMasking = new QGraphicsOpacityEffect();
    QBrush brushMask(playingMask);
    textMasking->setOpacity(0.9);
    textMasking->setOpacityMask(brushMask);
    maskingTextContainer->setGraphicsEffect(textMasking);
    scrollingTextContainer = new QGraphicsItemGroup(maskingTextContainer);


    QList<PlayerChord>::iterator ite;
    QGraphicsRectItem* tempCase;
    QGraphicsTextItem* tempText;
    QGraphicsTextItem* tempScrollingChord;

    int i=0, j=0;
    for (ite = bridge.chordList.begin(); ite != bridge.chordList.end(); ++ite)
    {

        // Partie chanson entiere
        tempCase = new QGraphicsRectItem(1471 + 355/4*i, 108 + 73*j, 355/4, 53, this);
        tempCase->setBrush(innerCont);
        tempCase->setPen(lightPen);
        cList.append(tempCase);

        tempText = new QGraphicsTextItem(tempCase);
        tempText->setFont(chordFont);
        tempText->setDefaultTextColor(QColor(14,153,204));
        tempText->setTextWidth(355/4);
        tempText->setHtml("<p align='center'>"+ite->name+"</p>");
        tempText->setPos(tempCase->rect().topLeft());
        if(i==3)
        {
            j++;
            i=0;
        }
        else
            i++;
        ite->fullSongItem = tempCase;

        // Partie accords defilants

        tempScrollingChord = new QGraphicsTextItem(scrollingTextContainer);
        tempScrollingChord->setFont(scrollingChordFont);
        tempScrollingChord->setPos(180 + ite->time*pixPerMsec, 440);
        tempScrollingChord->setHtml("<font color=\"#ffffff\">"+ite->name+"</font>");

        ite->scrollingChordItem = tempScrollingChord;

    }

    // partie temporelle
    time = 0;


}

void EntireSong::nextChord() {
    QBrush newCol(Qt::green);
    bridge.chordList[currentChord].fullSongItem->setBrush(newCol);
    currentChord++;
    if(currentChord>=bridge.chordList.size())
        currentChord=0;
}

void EntireSong::advance ( int phase ) {
    if(phase == 1)
    {
    int elapsed = Bridge::getElapsed();
    qDebug() << time;
    qDebug() << elapsed;
    qDebug() << elapsed-time;


        QTransform textTrans;
        textTrans.translate(qreal(-pixPerMsec*(elapsed-time)),0);
        scrollingTextContainer->setTransform(textTrans, true);

        //scrollingTextContainer->setPos(scrollingTextContainer->pos()-QPointF(elapsed*pixPerMsec, 0));
        if(bridge.chordList[currentChord].time<elapsed)
        {
            nextChord();
        }


    time = elapsed;
    }
}

void EntireSong::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

}

QRectF EntireSong::boundingRect() const {
    return QRectF(0,0,1920,1080);

}
