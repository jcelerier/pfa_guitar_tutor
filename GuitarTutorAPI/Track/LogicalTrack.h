#ifndef LOGICALTRACK_H
#define LOGICALTRACK_H

#include <QString>
#include "PartTrack.h"

class LogicalTrack {

public:

    LogicalTrack();
    ~LogicalTrack();

    void SetTrackName(QString newTrackName);
    QString GetTrackName();
    void SetArtist(QString newArtist);
    QString GetArtist();
    void SetAudioFileName(QString newAudioFileName);
    QString GetAudioFileName();
    QList<PartTrack*> getListPartTrack();

    PartTrack * AddPartTrackToList(QString partName);

private:

    QString trackName;
    QString artist;
    QString audioFileName;

    QList<PartTrack*> listPartTrack;

};

#endif // LOGICALTRACK_H
