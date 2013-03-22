#include "ScrollingItem.h"


#define PIXEL_PER_MSECOND 0.2

/**
 * @brief EntireSong::EntireSong
 * @param parent Parent de l'objet
 *
 * Construit le panneau récapitulatif de la track en cours d'exécution.
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
    QGraphicsSimpleTextItem* maskingTextContainer = new QGraphicsSimpleTextItem(this);
    maskingTextContainer->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    maskingTextContainer->setOpacity(0.01); // Pour ne pas voir le masque mais qu'il ne soit pas desactivé

    // Masque pour cacher ce qui est hors du cadre
    QGraphicsOpacityEffect* textMasking = new QGraphicsOpacityEffect();
    QBrush brushMask(playingMask);
    textMasking->setOpacity(0.9);
    textMasking->setOpacityMask(brushMask);
    maskingTextContainer->setGraphicsEffect(textMasking);
    m_scrollingTextContainer = new QGraphicsItemGroup(maskingTextContainer);

    m_scrollingTextContainer->setPos(m_initialPos);


}

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
 * @brief EntireSong::advance
 * @param phase
 *
 * Fonction de mise à jour du module. Demande le passage à l'accord suivant le cas échéant, et gère la validation de l'accord courant.
 */
void ScrollingItem::advance ( int phase )
{
    if(phase == 1) // advance est appellée automatiquement par la scene, deux fois (voir doc)
    {
        int currentTime = m_controler->elapsedTime();
        QTransform textTrans;
        textTrans.translate(-m_pixPerMsec*currentTime, 0);
        m_scrollingTextContainer->setTransform(textTrans);
    }
}

/**
 * @brief EntireSong::paint
 *
 * Non utilisée, mais nécessaire (méthode abstraite dans QGraphicsItem).
 */
void ScrollingItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
}

/**
 * @brief EntireSong::boundingRect
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
