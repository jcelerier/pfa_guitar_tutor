#ifndef PARTTRACK_H
#define PARTTRACK_H

#include <QList>
#include "TrackChord.h"

class PartTrack {

public:

    PartTrack();
    PartTrack(QString newPartName);
    ~PartTrack();

private:

    QString partName;
    QString annotation;
    int repetition;
    QList<TrackChord*> listTrackChords;

};

#endif // PARTTRACK_H
