#ifndef TRACKCHORD_H
#define TRACKCHORD_H

#include <QString>
#include "../GuitarTutor/Chord.h"

class TrackChord {

public:

    TrackChord();
    TrackChord(QString nom, qreal d, int rep);
    ~TrackChord();

    void setRepetition(int newRepetition);
    int getRepetition();

    qreal getDuration();
    QString getChord();

    QString toString();

private:

    Chord * currentChord;
    qreal duration;
    int repetition;

};

#endif // TRACKCHORD_H
