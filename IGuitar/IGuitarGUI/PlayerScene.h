#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include <QMap>

class PlayerScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PlayerScene(QWidget *parent = 0);
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
    QWidget *parent;

    void disposeScene();
    void switchPlaying();
};

#endif // MYSCENE_H
