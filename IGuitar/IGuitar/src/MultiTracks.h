/*
 * MultiTracks.h
 *
 *  Created on: 9 mars 2011
 *      Author: raphael
 */

#ifndef MULTITRACKS_H_
#define MULTITRACKS_H_

#include "Track.h"
#include <map>
#include <string>
#include <limits.h>


class MultiTracks {
public:
	MultiTracks(std::map<std::string, std::string> & tracks);
	MultiTracks(unsigned int silenceTimeInMs);
	virtual ~MultiTracks();

	void changeTrackMuteState(std::string trackName, bool muteState);

	float *getBuffer() const;
    int getBufferSize() const;
    int getNbChannels() const;

    void generateMusic();



private:
	std::map<std::string, Track> m_tracks;
	int m_bufferSize;
	int m_nbChannels;

	float *m_buffer;
};

#endif /* MULTITRACKS_H_ */
