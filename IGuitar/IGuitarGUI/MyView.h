#ifndef MYVIEW_H
#define MYVIEW_H

#include <QtWidgets/QGraphicsView>

#include "Configuration.h"

class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QGraphicsScene * scene, QWidget * parent = 0);
    void keyPressEvent(QKeyEvent* event);
signals:
    
public slots:
    
};

#endif // MYVIEW_H
