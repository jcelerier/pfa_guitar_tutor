#ifndef TRACKCHORD_H
#define TRACKCHORD_H

#include <QString>
#include "../GuitarTutor/Chord.h"

class PartTrack;

class TrackChord {
public:

	TrackChord();
	TrackChord(QString nom, qreal d, int rep, TrackChord* previous = 0, TrackChord* next = 0, PartTrack* part = 0);
	~TrackChord();

	void setRepetition(int newRepetition);
	int getRepetition();

	qreal getBeginningInMs();
	QString getChord();

	QString toString();

	void setPlaying(bool b=true);
	bool isPlaying() const;

	void setPlayed(bool b=true);
	bool isPlayed() const;

	void validate(bool b=true);
	bool isValidated() const;

	TrackChord* next();
	TrackChord* previous();
	PartTrack* part();

	void setNext(TrackChord*);
	void setPrevious(TrackChord*);
	void setPart(PartTrack*);


private:
	BasicChord * currentChord;
	qreal beginning_in_ms;
	int repetition;

	int m_num;
	PartTrack* m_part;
	TrackChord* m_next;
	TrackChord* m_previous;

	bool m_validated;
	bool m_played;
	bool m_playing;

};

#endif // TRACKCHORD_H
