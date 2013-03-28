#ifndef MENUITEM_H
#define MENUITEM_H

#include <QGraphicsRectItem>
#include "HelpWindow.h"
#include "ButtonItem.h"
#include "Configuration.h"


/**
 * @brief Menu de l'interface
 *
 * Est un objet de scèene gerant les signaux et les slots.
 * Contient les divers elements de menu
 *
 * @warning Penser a changer la taille de la boite de dialogue lors de l'ajout d'elements
 */
class MenuItem : public QGraphicsObject
{
Q_OBJECT
public:
	explicit MenuItem(QGraphicsItem *parent = 0);

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget *widget = 0);
	~MenuItem();
signals:

	void loadSong();
public slots:
	void closeMenu();
	void closeGame();

	void help();
private:
    int m_width; // Taille de la boite de menu
	int m_height;
    int m_nbElts; // Nombres d'elements dans le menu
    QGraphicsRectItem * m_rect; // Boite du menu
    QMap <QString, QGraphicsItem*> m_menuMap;   // Map des elements du menu
    HelpWindow* m_helpWindow;   // Boite d'aide
    QGraphicsRectItem* m_menuBack;  // Calque de fond masquant le reste de l'interface
    void addElt(QString, QString);  // Ajoute un element au menu
};

#endif // MENUITEM_H
