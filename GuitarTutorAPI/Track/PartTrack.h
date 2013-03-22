#ifndef PARTTRACK_H
#define PARTTRACK_H

#include <QList>
#include "TrackChord.h"

class PartTrack
{
	public:

		PartTrack();
		PartTrack(QString newPartName);
		PartTrack(QString newPartName, QList<TrackChord*> newListChords);
		QString getPartName();
		QList<TrackChord*> getTrackChordsList();
		void AddChord(TrackChord * c);
		~PartTrack();

		void incrementCurrentChordRepetition();

		void setNext(PartTrack*);
		void setPrevious(PartTrack*);
		PartTrack* next();
		PartTrack* previous();

	private:

		QString partName;
		QList<TrackChord*> listTrackChords;

		PartTrack * m_next;
		PartTrack * m_previous;

};

#endif // PARTTRACK_H
