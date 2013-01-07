#ifndef TRACKCHORD_H
#define TRACKCHORD_H

#include <QString>
#include "../GuitarTutor/Chord.h"

class TrackChord {

public:

    TrackChord();
    TrackChord(QString nom, int d, int rep);
    ~TrackChord();

    void setRepetition(int newRepetition);
    int getRepetition();
    int getDuration();
    Chord* getChord();


private:

    Chord * currentChord;
    int duration;
    int repetition;

};

#endif // TRACKCHORD_H
