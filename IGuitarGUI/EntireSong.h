#ifndef ENTIRESONG_H
#define ENTIRESONG_H

#include <QGraphicsItem>

class Controler;

class EntireSong : public QGraphicsItem
{
public:
	EntireSong(QGraphicsItem *parent = 0);
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget* widget= 0);
	void nextChord();
	virtual void advance ( int );
	void validateChord(bool v);
	QString getCurrentChord() const;
	int getCurrentDuration() const;
	int getTotalPlayedChords() const;
	int getTotalValidatedChords() const;
	bool getIsCurrentChordValidated() const;
	void setCurrentChord(int);

private:
	QList<QGraphicsRectItem*> m_cList;
	int m_currentChord;
    int m_lastRefresh;
	QGraphicsItemGroup* m_scrollingTextContainer;
	QGraphicsTextItem* m_timeText;
    QPointF m_initialPos;

	float m_pixPerMsec;

	Controler* m_controler;
	bool m_isCurrentChordValidated;

};

#endif // ENTIRESONG_H