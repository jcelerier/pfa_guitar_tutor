#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include <QMap>
#include "buttonitem.h"
#include "menuitem.h"
#include "entiresong.h"
#include "Configuration.h"

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
    void setPlayedChord(QString);
    void switchPlaying();
    void switchMenu();
    void openAudioOptions();

private:
    QRect windowSize;
    QMap <QString, QGraphicsItem*> itemMap;
    bool playing;
    Controler* controler;

    void disposeScene();

};

#endif // MYSCENE_H
