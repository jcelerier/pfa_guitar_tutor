#ifndef LOGICALTRACK_H
#define LOGICALTRACK_H

#include <QString>
#include "PartTrack.h"

class LogicalTrack {

public:

    LogicalTrack();
    LogicalTrack(QString newTrackName, QString newArtist, QString newAudoFileName, int newMesure, QList<PartTrack*> newList);
    LogicalTrack(QString newTrackName, QString newArtist, QString newAudoFileName, int newMesure, QList<PartTrack*> newList, int newBar, int newEnd, int newBeginning);
                 //, int newTimePerMesure);

    ~LogicalTrack();

    void setTrackName(QString newTrackName);
    QString getTrackName();

    void setArtist(QString newArtist);
    QString getArtist();

    void setAudioFileName(QString newAudioFileName);
    QString getAudioFileName();

    QList<PartTrack *> &getPartTrackList();

    void setMesure(int nb);
    void setBars(int nBar, int nBeginning, int nEnd);
    void setTimePerMesure(int nTPM);

    int getMesure();

	void setLine(int);
	void setColumn(int);

	int getLine();
	int getColumn();
    int getBeginning();
    int getBar();
    int getEnd();
    int getTimePerMesure();

    void addPartTrackToList(PartTrack* newPartTrack);
    QString getPartName(int n);
private:

    QString trackName;
    QString artist;
    QString audioFileName;
    int mesure;
	int line, column;
    int beginning, bar, end;
    int timePerMesure;

    QList<PartTrack*> listPartTrack;

};

#endif // LOGICALTRACK_H
