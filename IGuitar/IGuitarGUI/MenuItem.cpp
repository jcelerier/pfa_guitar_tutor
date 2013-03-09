#include "MenuItem.h"

/**
 * @brief MenuItem::MenuItem
 * @param parent Element parent
 *
 * Constructeur.
 */
MenuItem::MenuItem(QGraphicsItem *parent) :
	QGraphicsObject(parent),
	width(350),
	height(500)

{
	rect = new QGraphicsRectItem(this);

	// Hide fond modal
	rect->setRect(0, 0, Configuration::originalWidth, Configuration::originalHeight);

	QBrush hidingBrush(Qt::black);
	rect->setBrush(hidingBrush);
	rect->setOpacity(0.6);
	rect->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
	rect->setPanelModality(QGraphicsItem::SceneModal);

	QGraphicsRectItem* menuBack = new QGraphicsRectItem(rect);
	menuBack->setRect(Configuration::originalWidth/2-width/2, Configuration::originalHeight/2-width/2, width, height);

	QBrush backBrush(QColor(11,41,116));
	QPen borderPen(Qt::black, 6);
	menuBack->setBrush(backBrush);
	menuBack->setPen(borderPen);

	QFont eltFont("Roboto", 28);
	eltFont.setWeight(QFont::Bold);

	// Elements de menu
	QPixmap menuBtnImage(":/images/btn_bg.png");

	menuMap["closeMenu"] = new ButtonItem(menuBtnImage, rect);
	menuMap["closeMenu"]->setPos(menuBack->rect().topLeft()+QPointF(width/2 -272/2,20));
    menuMap["closeMenu"]->setToolTip(tr("Continue"));
	connect((ButtonItem*)menuMap["closeMenu"], SIGNAL(pushed()), this, SLOT(closeMenu()));

	menuMap["closeMenuText"] = new QGraphicsTextItem(rect);
	menuMap["closeMenuText"]->setPos(menuBack->rect().topLeft()+QPointF(width/2 -272/2,20+20));
	((QGraphicsTextItem*) menuMap["closeMenuText"])->setTextWidth(272);
	((QGraphicsTextItem*) menuMap["closeMenuText"])->setDefaultTextColor(QColor(0,95,61));
	((QGraphicsTextItem*) menuMap["closeMenuText"])->setFont(eltFont);
    ((QGraphicsTextItem*) menuMap["closeMenuText"])->setHtml(tr("<p align='center'>CONTINUE</p>"));

	menuMap["close"] = new ButtonItem(menuBtnImage, rect);
	menuMap["close"]->setPos(menuBack->rect().topLeft()+QPointF(width/2 -272/2,20+100));
    menuMap["close"]->setToolTip(tr("Quit"));
	connect((ButtonItem*)menuMap["close"], SIGNAL(pushed()), this, SLOT(closeGame()));

	menuMap["closeText"] = new QGraphicsTextItem(rect);
	menuMap["closeText"]->setPos(menuBack->rect().topLeft()+QPointF(width/2 -272/2,20+20+100));
	((QGraphicsTextItem*) menuMap["closeText"])->setTextWidth(272);
	((QGraphicsTextItem*) menuMap["closeText"])->setDefaultTextColor(QColor(0,95,61));
	((QGraphicsTextItem*) menuMap["closeText"])->setFont(eltFont);
    ((QGraphicsTextItem*) menuMap["closeText"])->setHtml(tr("<p align='center'>QUIT</p>"));
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
	return Configuration::getWindowSize();
}
