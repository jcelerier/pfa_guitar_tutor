#include "MenuItem.h"
#include "Controler.hpp"
#include "PlayerScene.h"

MenuItem::~MenuItem() {
	if(m_helpWindow != 0) {
		delete m_helpWindow;
		m_helpWindow = 0;
	}
	delete m_menuMap["closeMenu"];
	delete m_menuMap["closeMenuText"];
	delete m_menuMap["load"];
	delete m_menuMap["loadText"];
	delete m_menuMap["close"];
	delete m_menuMap["closeText"];
	delete m_menuMap["help"];
	delete m_menuMap["helpText"];
    delete m_menuMap["options"];
    delete m_menuMap["optionsText"];
	delete m_menuBack;
	delete m_rect;
}

/**
 * @brief MenuItem::MenuItem
 * @param parent Element parent
 *
 * Constructeur.
 */
MenuItem::MenuItem(QGraphicsItem *parent) :
	QGraphicsObject(parent),
	m_width(350),
	m_height(500),
	m_nbElts(0)

{
	m_rect = new QGraphicsRectItem(this);
	m_helpWindow = 0;

	// Hide fond modal
	m_rect->setRect(0, 0, Configuration::originalWidth, Configuration::originalHeight);
    m_rect->setZValue(50);

	QBrush hidingBrush(Qt::black);
	m_rect->setBrush(hidingBrush);
	m_rect->setOpacity(0.6);
	m_rect->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
	m_rect->setPanelModality(QGraphicsItem::SceneModal);

	m_menuBack = new QGraphicsRectItem(m_rect);
	m_menuBack->setRect(Configuration::originalWidth/2-m_width/2, Configuration::originalHeight/2-m_width/2, m_width, m_height);

	QBrush backBrush(QColor(11,41,116));
	QPen borderPen(Qt::black, 6);
	m_menuBack->setBrush(backBrush);
	m_menuBack->setPen(borderPen);

    QFont eltFont(":/fonts/Roboto-Regular.ttf", 28);
	eltFont.setWeight(QFont::Bold);

	// Elements de menu
	QPixmap closeBtnImage(":/images/cross.png");

	ButtonItem* closeBtn = new ButtonItem(closeBtnImage, m_rect);
	closeBtn->setPos(m_menuBack->rect().topRight()-QPointF(17,17));
	closeBtn->setToolTip(tr("Close"));
	connect((ButtonItem*)closeBtn, SIGNAL(pushed()), this, SLOT(closeMenu()));

	addElt("load", "CHARGER");
	connect((ButtonItem*)m_menuMap["load"], SIGNAL(pushed()), (Controler*) scene()->parent(), SLOT(initSong()));

    addElt("options", "OPTIONS");
    connect((ButtonItem*)m_menuMap["options"], SIGNAL(pushed()), (PlayerScene*) scene(), SLOT(displayOptions()));

	addElt("help", "AIDE");
	connect((ButtonItem*)m_menuMap["help"], SIGNAL(pushed()), this, SLOT(help()));

	addElt("close", "QUITTER");
	connect((ButtonItem*)m_menuMap["close"], SIGNAL(pushed()), this, SLOT(closeGame()));

}

/**
 * @brief MenuItem::addElt
 *
 * Méthode a appeler pour ajouter un element au menu
 */

void MenuItem::addElt(QString name, QString text) {
	if(m_menuMap.contains(name))
		return;

    QFont eltFont(":/fonts/Roboto-Regular.ttf", 28);
	eltFont.setWeight(QFont::Bold);
	QPixmap menuBtnImage(":/images/btn_bg.png");

	m_menuMap[name] = new ButtonItem(menuBtnImage, m_rect);
	m_menuMap[name]->setPos(m_menuBack->rect().topLeft()+QPointF(m_width/2 -272/2, 20 + m_nbElts*100));
	m_menuMap[name]->setToolTip(name);

	m_menuMap[name+"Text"] = new QGraphicsTextItem(m_rect);
	m_menuMap[name+"Text"]->setPos(m_menuBack->rect().topLeft()+QPointF(m_width/2 -272/2,20+20 + m_nbElts*100));
	((QGraphicsTextItem*) m_menuMap[name+"Text"])->setTextWidth(272);
	((QGraphicsTextItem*) m_menuMap[name+"Text"])->setDefaultTextColor(QColor(0,95,61));
	((QGraphicsTextItem*) m_menuMap[name+"Text"])->setFont(eltFont);
	((QGraphicsTextItem*) m_menuMap[name+"Text"])->setHtml("<p align='center'>"+text+"</p>");

	m_nbElts++;

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
	return m_menuBack->rect();
}

/**
 * @brief MenuItem::help
 *
 * Active la fenêtre d'aide
 */
void MenuItem::help()
{
	if(m_helpWindow != 0)
		delete m_helpWindow;
	m_helpWindow = new HelpWindow();
	m_helpWindow->exec();
}
