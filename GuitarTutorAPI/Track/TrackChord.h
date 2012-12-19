#ifndef TRACKCHORD_H
#define TRACKCHORD_H

#include <QString>
#include "../GuitarTutor/Chord.h"

class TrackChord {

public:

    TrackChord();
    TrackChord(QString nom, int t, int rep);
    ~TrackChord();

    void setRepetition(int newRepetition);
    int getRepetition();

private:

    Chord * currentChord;
    int time;
    int repetition;

};

#endif // TRACKCHORD_H
