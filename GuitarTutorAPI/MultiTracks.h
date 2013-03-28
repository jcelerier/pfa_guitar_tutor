/*
 * MultiTracks.h
 *
 *  Created on: 9 mars 2011
 *      Author: raphael
 */

#ifndef MULTITRACKS_H_
#define MULTITRACKS_H_

#include "Track.h"
#include <QMap>
#include <QString>

/**
 * @brief Classe servant à la gestion de plusieurs pistes à la suite (non utilisée pour plus d'une piste).
 */
class MultiTracks
{
public:
    MultiTracks(QMap<QString, QString> & tracks);
	MultiTracks(unsigned int silenceTimeInMs);
	virtual ~MultiTracks();

    void changeTrackMuteState(QString trackName, bool muteState);
    bool isTrackMute(QString trackName);

	float *getBuffer() const;
	int getBufferSize() const;
	int getNbChannels() const;

	void generateMusic();



private:
    QMap<QString, Track> m_tracks;
	int m_bufferSize;
	int m_nbChannels;

	float *m_buffer;
};

#endif /* MULTITRACKS_H_ */
