#ifndef LOGICALTRACK_H
#define LOGICALTRACK_H

#include <QString>
#include "PartTrack.h"

class LogicalTrack {

public:

    LogicalTrack();
    LogicalTrack(QString newTrackName, QString newArtist, QString newAudoFileName, int newMesure, QList<PartTrack*> newList);
    ~LogicalTrack();

    void setTrackName(QString newTrackName);
    QString getTrackName();

    void setArtist(QString newArtist);
    QString getArtist();

    void setAudioFileName(QString newAudioFileName);
    QString getAudioFileName();

	QList<PartTrack *> &getPartTrackList();

    void setMesure(int nb);
    int getMesure();

    void addPartTrackToList(PartTrack* newPartTrack);

private:

    QString trackName;
    QString artist;
    QString audioFileName;
    int mesure;

    QList<PartTrack*> listPartTrack;

};

#endif // LOGICALTRACK_H
