#ifndef PARTTRACK_H
#define PARTTRACK_H

#include <QList>
#include "TrackChord.h"

class PartTrack {

public:

    PartTrack();
    PartTrack(QString newPartName);
    QString getPartName();
    QList<TrackChord*> getListTrackChords();
    void AddChord(TrackChord * c);
    QString getName() const;
    ~PartTrack();

private:

    QString partName;
    QList<TrackChord*> listTrackChords;

};

#endif // PARTTRACK_H
