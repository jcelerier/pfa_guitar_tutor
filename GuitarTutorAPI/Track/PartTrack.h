#ifndef PARTTRACK_H
#define PARTTRACK_H

#include <QList>
#include "TrackChord.h"

/**
 * @brief Représente une partie d'un morceau sous forme d'une suite d'accord.
 * (par exemple l'intro ou un couplet).
 */

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

        QString m_partName;
        QList<TrackChord*> m_listTrackChords;

		PartTrack * m_next;
		PartTrack * m_previous;

};

#endif // PARTTRACK_H
