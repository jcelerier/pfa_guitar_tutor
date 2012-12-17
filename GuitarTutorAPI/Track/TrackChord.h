#ifndef TRACKCHORD_H
#define TRACKCHORD_H

#include <QString>
#include "../GuitarTutor/Chord.h"

class TrackChord {

public:

    TrackChord();
    ~TrackChord();

private:

    Chord CurrentChord;
    int Time;
    QString Annotation;

};

#endif // TRACKCHORD_H
