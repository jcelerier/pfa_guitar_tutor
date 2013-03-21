#ifndef STATEPACKET_H
#define STATEPACKET_H

#include "ChordTableWidget.h"

struct StatePacket
{

		ChordTableWidget *grid;
		QTime beginning, bar, end;
		QString artist, song;
		int chordsPerBar, timeSignature;

		StatePacket(ChordTableWidget *grid,
					QTime beginning, QTime bar, QTime end,
					QString artist, QString song,
					int chordsPerBar, int timeSignature);
};

#endif // STATEPACKET_H
