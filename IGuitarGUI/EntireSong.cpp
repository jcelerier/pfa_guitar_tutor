#include "EntireSong.h"
#include "Controler.hpp"
#include <QGraphicsOpacityEffect>
#include "PlayerScene.h"
#include "Util.hpp"

#define PIXEL_PER_MSECOND 0.2


/**
 * @brief EntireSong::EntireSong
 * @param parent Parent de l'objet
 *
 * Construit le panneau récapitulatif de la track en cours d'exécution.
 */
EntireSong::EntireSong(QGraphicsItem *parent) :
	QGraphicsItem(parent),
	m_currentChord(0),
    m_track(0),
    m_loaded(false)
{
	m_controler = (Controler*) scene()->parent();

	//QPen borderPen(Qt::black, 3);

}

/**
 * @brief EntireSong::nextChord
 *
 * Fonction appelée lors du début d'un nouvel accord. Elle gère la validation de l'accord qui vient de s'écouler.
 */
void EntireSong::nextChord() {
    paintChord(m_currentChord);

    m_currentChord=m_currentChord->next();
    m_gMap[m_currentChord]->setBrush(QColor(1, 174, 242));
}

/**
 * @brief EntireSong::paintChord
 *
 * Fonction appelée lors du début d'un nouvel accord. Elle gère la validation de l'accord qui vient de s'écouler.
 */
void EntireSong::paintChord(TrackChord * tc) {

    if( !m_currentChord->isPlayed() )
        m_gMap[tc]->setBrush(QColor(255,255,255));
    else if( m_currentChord->isValidated() )
        m_gMap[tc]->setBrush(QColor(101, 215, 78));
    else
        m_gMap[tc]->setBrush(QColor(175, 22, 27));
}

/**
 * @brief EntireSong::paint
 *
 * Non utilisée, mais nécessaire (méthode abstraite dans QGraphicsItem).
 */
void EntireSong::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
}

/**
 * @brief EntireSong::boundingRect
 * @return Le rectangle correspondant à la fenetre du player.
 *
 * Non utilisée, mais nécessaire (méthode abstraite dans QGrahicsItem).
 */
QRectF EntireSong::boundingRect() const {
	// This rectangle is false, but may decrease performance if determined precisely
	return QRectF(0,0,1920,1080);

}

/**
 * @brief EntireSong::setCurrentChord
 *
 * Actualise l'affichage avec l'accord en cours.
 */
void EntireSong::setCurrentChord(TrackChord* tc)
{
    if(m_currentChord !=0 && m_currentChord->next() == tc)
        // on update uniquement la case actuelle
        nextChord();
    else {
        // on retrace entierement
        TrackChord* iChord = m_track->getFirstChord();
        do
        {
            paintChord(iChord);
        } while((iChord = iChord->next()) != 0);

        m_gMap[tc]->setBrush(QColor(1, 174, 242));
        m_currentChord = tc;
    }
}


void EntireSong::load(LogicalTrack * lt) {
    QPen lightPen(Qt::black, 1);
    QBrush innerCont(Qt::white);
    QFont chordFont("Roboto", 32);

    m_track = lt;

    int i=0, j=0;
    QGraphicsRectItem* tempCase;
    QGraphicsTextItem* tempText;

    m_gMap.clear();

    TrackChord* iChord = m_track->getFirstChord();
    do
    {
        // Partie chanson entiere
        tempCase = new QGraphicsRectItem(1471 + 355/4*i, 108 + 73*j, 355/4, 53, this);
        tempCase->setBrush(innerCont);
        tempCase->setPen(lightPen);
        m_gMap[iChord] = tempCase;

        tempText = new QGraphicsTextItem(tempCase);
        tempText->setFont(chordFont);
        tempText->setDefaultTextColor(QColor(14,153,204));
        tempText->setTextWidth(355/4);
        tempText->setHtml("<p align='center'>"+stringToSub(iChord->getChord())+"</sub>"+"</p>");
        tempText->setPos(tempCase->rect().topLeft());

        if(i==3)
        {
            j++;
            i=0;
        }
        else
            i++;
    } while((iChord = iChord->next()) != 0);

    m_loaded = true;
    m_currentChord = m_track->getFirstChord();
}
