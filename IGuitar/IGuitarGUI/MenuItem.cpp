#include "MenuItem.h"

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
	menuMap["closeMenu"]->setToolTip("Close");
	connect((ButtonItem*)menuMap["closeMenu"], SIGNAL(pushed()), this, SLOT(closeMenu()));

	menuMap["closeMenuText"] = new QGraphicsTextItem(rect);
	menuMap["closeMenuText"]->setPos(menuBack->rect().topLeft()+QPointF(width/2 -272/2,20+20));
	((QGraphicsTextItem*) menuMap["closeMenuText"])->setTextWidth(272);
	((QGraphicsTextItem*) menuMap["closeMenuText"])->setDefaultTextColor(QColor(0,95,61));
	((QGraphicsTextItem*) menuMap["closeMenuText"])->setFont(eltFont);
	((QGraphicsTextItem*) menuMap["closeMenuText"])->setHtml("<p align='center'>FERMER MENU</p>");

	menuMap["close"] = new ButtonItem(menuBtnImage, rect);
	menuMap["close"]->setPos(menuBack->rect().topLeft()+QPointF(width/2 -272/2,20+100));
	menuMap["close"]->setToolTip("Close");
	connect((ButtonItem*)menuMap["close"], SIGNAL(pushed()), this, SLOT(closeGame()));

	menuMap["closeText"] = new QGraphicsTextItem(rect);
	menuMap["closeText"]->setPos(menuBack->rect().topLeft()+QPointF(width/2 -272/2,20+20+100));
	((QGraphicsTextItem*) menuMap["closeText"])->setTextWidth(272);
	((QGraphicsTextItem*) menuMap["closeText"])->setDefaultTextColor(QColor(0,95,61));
	((QGraphicsTextItem*) menuMap["closeText"])->setFont(eltFont);
	((QGraphicsTextItem*) menuMap["closeText"])->setHtml("<p align='center'>FERMER</p>");
}

void MenuItem::closeMenu()
{
	this->setVisible(false);
}

void MenuItem::closeGame()
{
	qApp->exit();
}

void MenuItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

}

QRectF MenuItem::boundingRect() const {
	return Configuration::getWindowSize();

}
