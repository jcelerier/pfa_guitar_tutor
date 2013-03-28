#include "StatePacket.h"

/**
 * @brief StatePacket::StatePacket
 *
 * Constructeur
 */
StatePacket::StatePacket(ChordTableWidget *grid,
						 int beginning, int bar, int end,
						 QString artist, QString song, QString comment,
						 int chordsPerBar, int timeSignature):
	m_beginning(beginning),
	m_bar(bar),
	m_end(end),
	m_artist(artist),
	m_song(song),
	m_comment(comment),
	m_chordsPerBar(chordsPerBar),
	m_timeSignature(timeSignature)
{
	this->m_grid = new ChordTableWidget(grid->columnCount(), grid->rowCount(), 0);
	grid->deepCopy(this->m_grid);
}
