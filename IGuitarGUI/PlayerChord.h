#ifndef PLAYERCHORD_H
#define PLAYERCHORD_H

#include <QGraphicsItem>
#include <Track/TrackChord.h>

/**
 * @brief The PlayerChord class
 *
 *
 */
class PlayerChord
{
public:
	QString getName() const;
	int getTime() const;
	void setName(const QString n);
	void setTime(const int t);
	QGraphicsRectItem* getFullSongItem() const;
	void setFullSongItem(QGraphicsRectItem *r);
	void setScrollingChordItem(QGraphicsTextItem *r);

	TrackChord* getTrackChord() const;
	void setTrackChord(TrackChord*);

private:
	QString m_name;
	int m_time;
	QGraphicsRectItem* m_fullSongItem;
	QGraphicsTextItem* m_scrollingChordItem;
	TrackChord* m_trackChord;

};

#endif // PLAYERCHORD_H
