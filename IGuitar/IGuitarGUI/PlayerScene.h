#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include <QMap>
#include "Bridge.h"
#include "buttonitem.h"
#include "menuitem.h"
#include "entiresong.h"
#include "config.h"

class Controler;

class PlayerScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PlayerScene(QObject *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    QGraphicsItem* getItem(QString);
    
signals:
    
public slots:
    void updateScene();
    void newChordPlayed(QString);

private:
    QRect windowSize;
    QMap <QString, QGraphicsItem*> itemMap;
    int framesPerSecond;
    bool playing;
    Bridge bridge;

    Controler* controler;

    void disposeScene();
public slots:
    void switchPlaying();
    void switchMenu();
};

#endif // MYSCENE_H
