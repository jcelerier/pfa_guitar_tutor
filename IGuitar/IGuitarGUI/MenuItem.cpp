#include "MenuItem.h"
#include "Controler.hpp"
/**
 * @brief MenuItem::MenuItem
 * @param parent Element parent
 *
 * Constructeur.
 */
MenuItem::MenuItem(QGraphicsItem *parent) :
	QGraphicsObject(parent),
	m_width(350),
	m_height(500)

{
	m_rect = new QGraphicsRectItem(this);

	// Hide fond modal
	m_rect->setRect(0, 0, Configuration::originalWidth, Configuration::originalHeight);

	QBrush hidingBrush(Qt::black);
	m_rect->setBrush(hidingBrush);
	m_rect->setOpacity(0.6);
	m_rect->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
	m_rect->setPanelModality(QGraphicsItem::SceneModal);

	QGraphicsRectItem* menuBack = new QGraphicsRectItem(m_rect);
	menuBack->setRect(Configuration::originalWidth/2-m_width/2, Configuration::originalHeight/2-m_width/2, m_width, m_height);

	QBrush backBrush(QColor(11,41,116));
	QPen borderPen(Qt::black, 6);
	menuBack->setBrush(backBrush);
	menuBack->setPen(borderPen);

	QFont eltFont("Roboto", 28);
	eltFont.setWeight(QFont::Bold);

	// Elements de menu
	QPixmap menuBtnImage(":/images/btn_bg.png");

	m_menuMap["closeMenu"] = new ButtonItem(menuBtnImage, m_rect);
	m_menuMap["closeMenu"]->setPos(menuBack->rect().topLeft()+QPointF(m_width/2 -272/2,20));
    m_menuMap["closeMenu"]->setToolTip(tr("Continue"));
	connect((ButtonItem*)m_menuMap["closeMenu"], SIGNAL(pushed()), this, SLOT(closeMenu()));

	m_menuMap["closeMenuText"] = new QGraphicsTextItem(m_rect);
	m_menuMap["closeMenuText"]->setPos(menuBack->rect().topLeft()+QPointF(m_width/2 -272/2,20+20));
	((QGraphicsTextItem*) m_menuMap["closeMenuText"])->setTextWidth(272);
	((QGraphicsTextItem*) m_menuMap["closeMenuText"])->setDefaultTextColor(QColor(0,95,61));
	((QGraphicsTextItem*) m_menuMap["closeMenuText"])->setFont(eltFont);
    ((QGraphicsTextItem*) m_menuMap["closeMenuText"])->setHtml(tr("<p align='center'>CONTINUE</p>"));

	m_menuMap["close"] = new ButtonItem(menuBtnImage, m_rect);
    m_menuMap["close"]->setPos(menuBack->rect().topLeft()+QPointF(m_width/2 -272/2,20+100+100));
    m_menuMap["close"]->setToolTip(tr("Quit"));
	connect((ButtonItem*)m_menuMap["close"], SIGNAL(pushed()), this, SLOT(closeGame()));

	m_menuMap["closeText"] = new QGraphicsTextItem(m_rect);
    m_menuMap["closeText"]->setPos(menuBack->rect().topLeft()+QPointF(m_width/2 -272/2,20+20+100+100));
	((QGraphicsTextItem*) m_menuMap["closeText"])->setTextWidth(272);
	((QGraphicsTextItem*) m_menuMap["closeText"])->setDefaultTextColor(QColor(0,95,61));
	((QGraphicsTextItem*) m_menuMap["closeText"])->setFont(eltFont);
    ((QGraphicsTextItem*) m_menuMap["closeText"])->setHtml(tr("<p align='center'>QUIT</p>"));

    m_menuMap["closeText"] = new ButtonItem(menuBtnImage, m_rect);
    m_menuMap["closeText"]->setPos(menuBack->rect().topLeft()+QPointF(m_width/2 -272/2,20+100));
    m_menuMap["closeText"]->setToolTip(tr("Load"));
    connect((ButtonItem*)m_menuMap["loadSong"], SIGNAL(pushed()), this, SLOT(loadSong()));

    m_menuMap["closeText"] = new QGraphicsTextItem(m_rect);
    m_menuMap["closeText"]->setPos(menuBack->rect().topLeft()+QPointF(m_width/2 -272/2,20+20+100));
    ((QGraphicsTextItem*) m_menuMap["closeText"])->setTextWidth(272);
    ((QGraphicsTextItem*) m_menuMap["closeText"])->setDefaultTextColor(QColor(0,95,61));
    ((QGraphicsTextItem*) m_menuMap["closeText"])->setFont(eltFont);
    ((QGraphicsTextItem*) m_menuMap["closeText"])->setHtml(tr("<p align='center'>LOAD</p>"));
}

/**
 * @brief MenuItem::closeMenu
 *
 * Méthode a appeler pour fermer le menu.
 */
void MenuItem::closeMenu()
{
	this->setVisible(false);
}

/**
 * @brief MenuItem::closeGame
 *
 * Quitte l'application.
 */
void MenuItem::closeGame()
{
	qApp->exit();
}

/**
 * @brief MenuItem::loadSong
 *
 * Ouvre la fenêtre de chargement d'un morceau.
 */
void MenuItem::loadSong()
{

    /*if(!Controler::initSong())
        qDebug() << "Controler::initSong()";*/
}

/**
 * @brief MenuItem::paint
 *
 * Non utilisée, mais nécessaire (méthode abstraite dans QGraphicsItem).
 */
void MenuItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){

}

/**
 * @brief MenuItem::boundingRect
 * @return La taille de l'écran.
 *
 * Retourne la taille de l'écran de l'utilisateur.
 */
QRectF MenuItem::boundingRect() const {
	return Configuration::getWindowSize();
}
