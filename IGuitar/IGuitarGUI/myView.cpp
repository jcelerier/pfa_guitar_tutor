#include "myView.h"
#include "config.h"

myView::myView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene, parent)
{
    centerOn(960,540);
    scale(Config::quotient, Config::quotient);
    setSceneRect(0, 0, 1900, 1060);
    //setSceneRect(Config::getWindowSize());
    //setFixedSize(Config::getWindowSize().size());

    //fitInView(Config::getWindowSize());


    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing, true);

    showFullScreen();
}

void myView::keyPressEvent(QKeyEvent* event)
{
    QGraphicsView::keyPressEvent(event);


    switch ( event->key() )
     {
        case Qt::Key_Up :
            scale(0.5,0.5);
            break;
        case Qt::Key_Down :
            scale(2,2);
            break;
        case Qt::Key_Right :
            scene()->advance();
            break;
    }
    event->accept();
}
