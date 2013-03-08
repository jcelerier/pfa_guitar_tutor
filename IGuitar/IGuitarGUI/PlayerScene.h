#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>
#include <QMap>
#include "ButtonItem.h"
#include "MenuItem.h"
#include "EntireSong.h"
#include "Configuration.h"

class Controler;

class PlayerScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit PlayerScene(QObject *parent = 0);
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	QGraphicsItem* getItem(QString);
	void setCurrentChordValidated(bool v);
    void resetNoteCheck();
    void updateStats(int validated, int played);
signals:

public slots:
	void updateScene();
	void setPlayedChord(QString);
	void switchPlaying();
	void switchMenu();

private:
	QRect windowSize;
	QMap <QString, QGraphicsItem*> itemMap;
	bool playing;
	bool valideNote;
	int  timeNoteSynchronized;
	int  lastTimeCheck;
	int  currentNoteDuration;
	Controler* controler;

	void disposeScene();

};

#endif // MYSCENE_H
