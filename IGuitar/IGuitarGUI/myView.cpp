#include "myView.h"

myView::myView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene, parent)
{
    centerOn(Configuration::originalWidth/2, Configuration::originalHeight/2);
    scale(Configuration::quotient, Configuration::quotient);
    setSceneRect(0, 0, Configuration::originalWidth-20, Configuration::originalHeight-20);
    //setSceneRect(Config::getWindowSize());
    //setFixedSize(Config::getWindowSize().size());

    //fitInView(Config::getWindowSize());


    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing);
    //setRenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

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