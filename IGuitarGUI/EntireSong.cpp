#include "EntireSong.h"
#include "Controler.hpp"
#include <QGraphicsOpacityEffect>
#include "PlayerScene.h"

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
	m_pixPerMsec(PIXEL_PER_MSECOND)
{
	m_controler = (Controler*) scene()->parent();

	//QPen borderPen(Qt::black, 3);
	QPen lightPen(Qt::black, 1);
	QBrush innerCont(Qt::white);
	QFont chordFont("Roboto", 32);
	QFont scrollingChordFont("Roboto", 60);

   /* QGraphicsRectItem* lineCont = new QGraphicsRectItem(1470, 108, 355, 53, this);
	lineCont->setBrush(innerCont);
	lineCont->setPen(borderPen);*/


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
	m_scrollingTextContainer->setPos(210, 440);


	QList<PlayerChord>::iterator ite;
	QGraphicsRectItem* tempCase;
	QGraphicsTextItem* tempText;
	QGraphicsTextItem* tempScrollingChord;

	int i=0, j=0;
	for (ite = m_controler->getChordList()->begin(); ite != m_controler->getChordList()->end(); ++ite)
	{

		// Partie chanson entiere
		tempCase = new QGraphicsRectItem(1471 + 355/4*i, 108 + 73*j, 355/4, 53, this);
		tempCase->setBrush(innerCont);
		tempCase->setPen(lightPen);
		m_cList.append(tempCase);

		tempText = new QGraphicsTextItem(tempCase);
		tempText->setFont(chordFont);
		tempText->setDefaultTextColor(QColor(14,153,204));
		tempText->setTextWidth(355/4);
		tempText->setHtml("<p align='center'>"+ite->getName()[0]+"<sub>"+ite->getName().mid(1)+"</sub>"+"</p>");
		tempText->setPos(tempCase->rect().topLeft());
		if(i==3)
		{
			j++;
			i=0;
		}
		else
			i++;
		ite->setFullSongItem(tempCase);

		// Partie accords defilants

		tempScrollingChord = new QGraphicsTextItem(m_scrollingTextContainer);
		tempScrollingChord->setFont(scrollingChordFont);
		tempScrollingChord->setPos(ite->getTime()*m_pixPerMsec, 0);
		tempScrollingChord->setHtml("<font color=\"#ffffff\">"+ite->getName()+"</font>");

		ite->setScrollingChordItem(tempScrollingChord);

	}
	// partie temporelle
	m_lastRefresh = 0;
	m_controler->getChordList()->at(m_currentChord).getFullSongItem()->setBrush(QColor(1, 174, 242));
	m_isCurrentChordValidated = false;
	m_totalPlayedChords = 0;
	m_totalValidatedChords = 0;
}

/**
 * @brief EntireSong::nextChord
 *
 * Fonction appelée lors du début d'un nouvel accord. Elle gère la validation de l'accord qui vient de s'écouler.
 */
void EntireSong::nextChord() {
	((PlayerScene*) scene())->resetNoteCheck();
	if(!m_isCurrentChordValidated)
		validateChord(false);
	m_isCurrentChordValidated = false;
	m_currentChord++;
	m_controler->getChordList()->at(m_currentChord).getFullSongItem()->setBrush(QColor(1, 174, 242));
	if(m_currentChord>=m_controler->getChordList()->size())
		m_currentChord=0;
}

/**
 * @brief EntireSong::advance
 * @param phase
 *
 * Fonction de mise à jour du module. Demande le passage à l'accord suivant le cas échéant, et gère la validation de l'accord courant.
 */
void EntireSong::advance ( int phase ) {
	if(phase == 1) // advance est appellée automatiquement par la scene, deux fois (voir doc)
	{
		int currentTime = m_controler->elapsedTime();
		static bool isLastChordAlreadyValidated = false;
		QTransform textTrans;
		textTrans.translate(-m_pixPerMsec*(currentTime-m_lastRefresh), 0);
		m_scrollingTextContainer->setTransform(textTrans, true);

		if(m_currentChord+1 < m_controler->getChordList()->size()) {
			if(m_controler->getChordList()->at(m_currentChord+1).getTime()<currentTime)
				nextChord();
		}
		else if(!isLastChordAlreadyValidated && !m_isCurrentChordValidated
				&& m_controler->getChordList()->at(m_currentChord).getTime() + getCurrentDuration() < currentTime) {//Dernier accord du morceau
			validateChord(false);
			isLastChordAlreadyValidated = true;
		}
		m_lastRefresh = currentTime;
	}
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
 * @brief EntireSong::validateChord
 * @param v Vrai si et seulement si l'accord courant doit etre validé.
 *
 * Méthode à appeler pour valider ou non l'accord courant. Se charge de colorier l'accord dans le module et de mettre à jour les statistiques de l'interface.
 */
void EntireSong::validateChord(bool v)
{
	if(v) {
		m_controler->getChordList()->at(m_currentChord).getFullSongItem()->setBrush(QColor(101, 215, 78));
		m_isCurrentChordValidated = true;
		m_totalValidatedChords++;
	}
	else
		m_controler->getChordList()->at(m_currentChord).getFullSongItem()->setBrush(QColor(175, 22, 27));
	m_totalPlayedChords++;
	((PlayerScene*)scene())->updateStats(m_totalValidatedChords, m_totalPlayedChords);
}

/**
 * @brief EntireSong::getCurrentChord
 * @return Le nom de l'accord devant etre joué.
 *
 * Retourne le nom de l'accord courant dans la partition.
 */
QString EntireSong::getCurrentChord() const
{
	return m_controler->getChordList()->at(m_currentChord).getName();
}

/**
 * @brief EntireSong::getCurrentDuration
 * @return La durée de l'accord courant.
 *
 * Retourne la durée de l'accord courant dans la partition.
 * @todo Durée du dernier accord.
 */
int EntireSong::getCurrentDuration() const
{
	if(m_currentChord+1 < m_controler->getChordList()->size())
		return m_controler->getChordList()->at(m_currentChord+1).getTime() - m_controler->getChordList()->at(m_currentChord).getTime();
	return 2000; //TODO
}

/**
 * @brief EntireSong::getTotalValidatedChords
 * @return Le nombre total d'accords joués et validés.
 *
 * Retourne le nombre total d'accords joués et validés jusqu'à présent. Permet la mise à jour ou la consultation des statistiques.
 */
int EntireSong::getTotalValidatedChords() const
{
	return m_totalValidatedChords;
}

/**
 * @brief EntireSong::getTotalPlayedChords
 * @return Le nombre total d'accords joués.
 *
 * Retourne le nombre total d'accords joués jusqu'à présent. Permet la mise à jour ou la consultation des statistiques.
 */
int EntireSong::getTotalPlayedChords() const
{
	return m_totalPlayedChords;
}

/**
 * @brief EntireSong::getIsCurrentChordValidated
 * @return Vrai si et seulement si l'accord courant a déjà été validé.
 *
 * Indique si l'utilisateur a déjà validé ou non l'accord courant.
 */
bool EntireSong::getIsCurrentChordValidated() const
{
	return m_isCurrentChordValidated;
}

/**
 * @brief EntireSong::setCurrentChord
 *
 * Actualise l'affichage avec l'accord en cours.
 */
void EntireSong::setCurrentChord(int cc)
{
    for(int i=0; i<m_controler->getChordList()->size(); i++) {
        m_controler->getChordList()->at(i).getFullSongItem()->setBrush(QColor(255, 255, 255));
    }
    for(int i=0; i<cc-1; i++) {
        m_controler->getChordList()->at(i).getFullSongItem()->setBrush(QColor(175, 22, 27)); // ne verifie pas encore si a été validé ou non
    }
}
