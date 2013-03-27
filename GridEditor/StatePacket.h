#ifndef STATEPACKET_H
#define STATEPACKET_H

#include "ChordTableWidget.h"
/**
 * @brief The StatePacket struct
 *
 * Cette structure représente l'état de l'éditeur à un instant donné.
 */
struct StatePacket
{

		ChordTableWidget *grid;
		int beginning, bar, end;
		QString artist, song, comment;
		int chordsPerBar, timeSignature;

		StatePacket(ChordTableWidget *grid,
					int beginning, int bar, int end,
					QString artist, QString song, QString comment,
					int chordsPerBar, int timeSignature);
};

#endif // STATEPACKET_H
