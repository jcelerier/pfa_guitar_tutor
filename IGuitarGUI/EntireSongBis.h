#ifndef ENTIRESONGBIS_H
#define ENTIRESONGBIS_H

#include <QGraphicsItem>
#include <Track/LogicalTrack.h>

class EntireSongBis : public QObject, public QGraphicsItem
{
		Q_OBJECT
	public:
		EntireSongBis(QGraphicsItem* parent = 0);
		virtual ~EntireSongBis();
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget* widget= 0);

		void load(LogicalTrack* track);
		void drawChord(QPainter* painter, int x, int y, TrackChord* accord);
		void drawPart(QPainter* painter, int x, int y, QString part);

	public slots:
		void repaintSong();
	private:
		QPen* m_borderPen, *m_notPlayedPen, *m_failedPen, *m_notFailedPen, *m_currentPen;
		LogicalTrack* m_track;
		bool m_loaded;
		QFont* chordFont, *partFont;

		static const int m_absoluteX = 1480;
		static const int m_absoluteY = 100;
		static const int m_caseWidth = 80;
		static const int m_caseHeight = 70;
		static const int m_heightSpacing = 50;


};

#endif // ENTIRESONGBIS_H
