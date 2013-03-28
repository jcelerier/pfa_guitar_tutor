#ifndef LOGICALTRACK_H
#define LOGICALTRACK_H

#include <QString>
#include "PartTrack.h"

/**
 * @brief Représente un morceau en mémoire
 *
 * Ces attributs représentent les différentes informations mémorisées.
 */

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

    void setComment(QString m_comment);
	QString getComment();

	void setAudioFileName(QString newAudioFileName);
	QString getAudioFileName();

	QList<PartTrack *> &getPartTrackList();

	void setMesure(int nb);
	void setBars(int nBar, int nBeginning, int nEnd);
	void setTimeSignature(int nTS);

    int getChordsPerBar();

	void setLine(int);
	void setColumn(int);

	int getLine();
	int getColumn();
	int getBeginning();
	int getBar();
	int getEnd();

	int getTimeSignature();
	int getBPM();

	int getPositionOfChordInMs(TrackChord*);


	void addPartTrackToList(PartTrack* newPartTrack);
	QString getPartName(int n);

	TrackChord* getFirstChord();

private:
    QString m_comment;
    QString m_trackName;
    QString m_artist;
    QString m_audioFileName;
    int m_chordsPerBar;
    int m_line, m_column;
    int m_beginning, m_bar, m_end;
    unsigned int m_timeSignature;

    QList<PartTrack*> m_listPartTrack;

};

#endif // LOGICALTRACK_H
