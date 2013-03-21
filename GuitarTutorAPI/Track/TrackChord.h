#ifndef TRACKCHORD_H
#define TRACKCHORD_H

#include <QString>
#include "../GuitarTutor/Chord.h"

class PartTrack;

class TrackChord {
public:

	TrackChord();
	TrackChord(QString nom, qreal d, int rep);
	~TrackChord();

	void setRepetition(int newRepetition);
	int getRepetition();

	qreal getBeginningInMs();
	QString getChord();

	QString toString();

private:
	BasicChord * currentChord;
	qreal beginning_in_ms;
	int repetition;

	int m_num;
	PartTrack* m_part;

};

#endif // TRACKCHORD_H
