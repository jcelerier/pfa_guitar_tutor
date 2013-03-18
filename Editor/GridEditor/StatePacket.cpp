#include "StatePacket.h"

StatePacket::StatePacket(ChordTableWidget *grid,
						 QTime beginning, QTime bar, QTime end,
						 QString artist, QString song,
						 int chordsPerBar, int timeSignature):
	beginning(beginning),
	bar(bar),
	end(end),
	artist(artist),
	song(song),
	chordsPerBar(chordsPerBar),
	timeSignature(timeSignature)
{
	this->grid = new ChordTableWidget(grid->columnCount(), grid->rowCount(), 0);
	grid->deepCopy(this->grid);
}
