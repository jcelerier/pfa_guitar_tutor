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
    void setMesure(int nb);
    QList<PartTrack*> getListPartTrack();
    void AddPartTrackToList(PartTrack* newPartTrack);

private:

    QString trackName;
    QString artist;
    QString audioFileName;
    int mesure;

    QList<PartTrack*> listPartTrack;

};

#endif // LOGICALTRACK_H
