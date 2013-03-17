#ifndef MENUITEM_H
#define MENUITEM_H

#include <QGraphicsRectItem>
#include "HelpWindow.h"
#include "ButtonItem.h"
#include "Configuration.h"


/**
 * @brief Menu de l'interface
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
    
public slots:
    void closeMenu();
    void closeGame();
    void loadSong();
    void help();
private:
    int m_width;
    int m_height;
    QGraphicsRectItem * m_rect;
    QMap <QString, QGraphicsItem*> m_menuMap;
    HelpWindow* m_helpWindow;
    QGraphicsRectItem* m_menuBack;
};

#endif // MENUITEM_H
