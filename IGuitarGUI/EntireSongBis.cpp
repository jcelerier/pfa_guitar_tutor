#include "EntireSongBis.h"
#include <QPainter>

EntireSongBis::EntireSongBis(QGraphicsItem* parent): QGraphicsItem(parent),
	m_track(0), m_loaded(false)
{
	m_borderPen = new QPen(Qt::black , 1, Qt::SolidLine, Qt::RoundCap);
	m_notPlayedPen = new QPen(Qt::white , 1, Qt::SolidLine, Qt::RoundCap);
	m_failedPen = new QPen(Qt::red , 1, Qt::SolidLine, Qt::RoundCap);
	m_notFailedPen = new QPen(Qt::green , 1, Qt::SolidLine, Qt::RoundCap);

	m_currentPen = new QPen(Qt::blue , 1, Qt::SolidLine, Qt::RoundCap);

	chordFont = new QFont(":/fonts/Roboto-Regular.ttf", 32);
	partFont = new QFont(":/fonts/Roboto-Regular.ttf", 17);
}

EntireSongBis::~EntireSongBis()
{

}

void EntireSongBis::load(LogicalTrack* track)
{
	m_track = track;
	m_loaded = true;
}

void EntireSongBis::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if(m_loaded)
	{
		int i = 0, j = 0, max_j = 4;
		TrackChord* chord = m_track->getPartTrackList()[0]->getTrackChordsList()[0];
		do
		{
			if(chord->getChord() != "n")
				drawChord(painter, j, i, chord);

			if(chord->previous() == 0)
			{
				drawPart(painter, j, i, chord->part()->getPartName());
			}
			else if (chord->part()->getTrackChordsList()[0] == chord)
			{
				drawPart(painter, j, i, chord->part()->getPartName());
			}

			if(max_j == ++j)
			{
				j = 0;
				i++;
			}

		} while((chord = chord->next()) != 0);
	}
}

void EntireSongBis::drawPart(QPainter* painter, int x, int y, QString part)
{
	int x0 = m_absoluteX + x * (m_caseWidth + 2);
	int y0 = m_absoluteY + y * (m_caseHeight + m_heightSpacing);

	painter->setPen(*m_borderPen);
	painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));

	QRect baseRect(x0 + 2, y0 + 43, m_caseWidth-2, m_caseHeight-2);

	painter->setFont(*partFont);
	painter->drawText(baseRect, part);
}

void EntireSongBis::drawChord(QPainter* painter, int x, int y, TrackChord* chord)
{
	int x0 = m_absoluteX + x * (m_caseWidth + 2);
	int y0 = m_absoluteY + y * (m_caseHeight + m_heightSpacing);

	painter->setPen(*m_borderPen);
	painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));

	painter->drawRect(x0, y0, m_caseWidth, m_caseHeight);

	if(chord->isPlayed())
	{
		if(chord->isValidated())
		{
			painter->setPen(*m_notFailedPen);
			painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
		}
		else
		{
			painter->setPen(*m_failedPen);
			painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
		}
	}
	else if(chord->isPlaying())
	{
		painter->setPen(*m_currentPen);
		painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
	}
	else
	{
		painter->setPen(*m_notPlayedPen);
		painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
	}

	QRect baseRect(x0+2, y0+2, m_caseWidth-2, m_caseHeight-2);
	painter->drawRect(baseRect);

	QRect textRect(baseRect);
	textRect.adjust(0, -15, 0, 0);
	painter->setPen(*m_borderPen);
	painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
	painter->setFont(*chordFont);
	painter->drawText(textRect, Qt::AlignCenter,
					  chord->getChord());

}

QRectF EntireSongBis::boundingRect() const {
	// This rectangle is false, but may decrease performance if determined precisely
	return QRectF(0,0,1920,1080);

}
void EntireSongBis::repaintSong()
{
	update(1480, 100, 440, 980 );
}
