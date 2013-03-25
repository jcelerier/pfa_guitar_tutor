#include "ScrollingItem.h"


#define PIXEL_PER_MSECOND 0.2

/**
 * @brief ScrollingItem::ScrollingItem
 * @param parent Parent de l'objet
 *
 * Construit la liste des accords défilants.
 */
ScrollingItem::ScrollingItem(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    m_initialPos(qreal(210), qreal(440)),
    m_pixPerMsec(PIXEL_PER_MSECOND),
    m_track(0),
    m_loaded(false)
{
    m_controler = (Controler*) scene()->parent();

    // Conteneur pour les accords défilants
    QPixmap playingMask(":/images/maskplaying.png");
    QBrush brushMask(playingMask);
    QGraphicsOpacityEffect* textMasking = new QGraphicsOpacityEffect(); // Masque pour cacher ce qui est hors du cadre
    textMasking->setOpacity(0.9);
    textMasking->setOpacityMask(brushMask);

    QGraphicsSimpleTextItem* maskingTextContainer = new QGraphicsSimpleTextItem(this);
    maskingTextContainer->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    maskingTextContainer->setOpacity(0.9); // Pour ne pas voir le masque mais qu'il ne soit pas desactivé
    maskingTextContainer->setGraphicsEffect(textMasking);

    m_scrollingTextContainer = new QGraphicsItemGroup(maskingTextContainer);
    /*
    // Conteneur pour les accords défilants
    QPixmap playingMask(":/images/maskplaying.png");
    QBrush brushMask(playingMask);
    QGraphicsOpacityEffect* textMasking = new QGraphicsOpacityEffect(); // Masque pour cacher ce qui est hors du cadre
    textMasking->setOpacityMask(brushMask);

    m_scrollingTextContainer = new QGraphicsItemGroup(this);
    m_scrollingTextContainer->setGraphicsEffect(textMasking);
*/
    m_scrollingTextContainer->setPos(m_initialPos);


}

/**
 * @brief ScrollingItem::load
 * @param lt Track à charger
 *
 * Charge les accords d'une track dans le conteneur.
 */
void ScrollingItem::load(LogicalTrack * lt) {
    m_track = lt;

    QFont scrollingChordFont("Roboto", 60);

    QGraphicsTextItem* tempScrollingChord;

    TrackChord* chord = m_track->getPartTrackList()[0]->getTrackChordsList()[0];
    do
    {
        if(chord->getChord() != "n") {

            tempScrollingChord = new QGraphicsTextItem(m_scrollingTextContainer);
            tempScrollingChord->setFont(scrollingChordFont);
            tempScrollingChord->setPos(chord->getBeginningInMs()*m_pixPerMsec, 0);
            tempScrollingChord->setHtml("<font color=\"#ffffff\">"+stringToSub(chord->getChord())+"</font>");
        }

    } while((chord = chord->next()) != 0);

    m_loaded = true;
}

/**
 * @brief ScrollingItem::advance
 * @param phase
 *
 * Fait avancer la liste des accords.
 */
void ScrollingItem::advance ( int phase )
{
    if(phase == 1) // advance est appellée automatiquement par la scene, deux fois (voir doc)
    {
        int currentTime = m_controler->elapsedTime();
        QTransform textTrans;
        textTrans.translate(-m_pixPerMsec*currentTime, 0); // methode 1
        //textTrans.setMatrix(0,0,0,0,0,0,-m_pixPerMsec*currentTime,0,0); // methode 2
        m_scrollingTextContainer->setTransform(textTrans);
    }
}

/**
 * @brief ScrollingItem::paint
 *
 * Non utilisée, mais nécessaire (méthode abstraite dans QGraphicsItem).
 */
void ScrollingItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
}

/**
 * @brief ScrollingItem::boundingRect
 * @return Le rectangle correspondant à la fenetre du player.
 *
 * Non utilisée, mais nécessaire (méthode abstraite dans QGrahicsItem).
 */
QRectF ScrollingItem::boundingRect() const {
    // This rectangle is false, but may decrease performance if determined precisely
    return QRectF(0,0,1920,1080);

}

/**
 * @brief ScrollingItem::setCurrentChord
 *
 * Actualise l'affichage avec l'accord en cours.
 */
void ScrollingItem::setCurrentChord(TrackChord* tc)
{
    QTransform textTrans;
    textTrans.translate(-m_pixPerMsec* tc->getBeginningInMs(), 0);
    m_scrollingTextContainer->setTransform(textTrans);
}
