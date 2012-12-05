/*
 * Track.h
 *
 *  Created on: 9 mars 2011
 *      Author: raphael
 */

#ifndef TRACK_H_
#define TRACK_H_

#include <string>
#include <iostream>
#include <sndfile.h>
#include <vector>

#define SAMPLE_RATE  (44100)
#define OUTPUT_FRAMES_PER_BUFFER (512)
#define INPUT_FRAMES_PER_BUFFER (4096)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (2)

#define DITHER_FLAG     (0)

/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#endif

class Track {
public:
	Track();
	Track(std::string fileName);
	Track(unsigned int silenceTrackTime);
	virtual ~Track();

	bool isMute() const;
	void setMuteState(bool isMute);

	float* getBuffer() const;
	int getChannelsCount() const;
	int getFramesCount() const;
	void setFramesCount(int m_framesCount);

private:
	bool m_isMute;
	int m_channelsCount;
	int m_framesCount;

	std::vector<float> m_tempBuffer;

	std::string m_fileName;
	float *m_buffer;

	int load();
};

#endif /* TRACK_H_ */
