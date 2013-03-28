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
	beginning(beginning),
	bar(bar),
	end(end),
	artist(artist),
	song(song),
	comment(comment),
	chordsPerBar(chordsPerBar),
	timeSignature(timeSignature)
{
	this->grid = new ChordTableWidget(grid->columnCount(), grid->rowCount(), 0);
	grid->deepCopy(this->grid);
}
