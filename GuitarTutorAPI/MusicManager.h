/*
 * MusicManager.h
 *
 *  Created on: 10 mars 2011
 *      Author: raphael
 */

#ifndef MUSICMANAGER_H_
#define MUSICMANAGER_H_

#include <map>
#include <string>
#include <boost/thread.hpp>
#include <sndfile.h>

#include "MultiTracks.h"
#include <portaudio.h>

extern "C" {
#include "chord/chord_c.h"
}



//#elif 1
//#define PA_SAMPLE_TYPE  paInt16
//typedef short SAMPLE;
//#define SAMPLE_SILENCE  (0)
//#define PRINTF_S_FORMAT "%d"
//#elif 0
//#define PA_SAMPLE_TYPE  paInt8
//typedef char SAMPLE;
//#define SAMPLE_SILENCE  (0)
//#define PRINTF_S_FORMAT "%d"
//#else
//#define PA_SAMPLE_TYPE  paUInt8
//typedef unsigned char SAMPLE;
//#define SAMPLE_SILENCE  (128)
//#define PRINTF_S_FORMAT "%d"

class MusicManager;
typedef struct
{
	static const unsigned short NB_CROSSFADE_FRAMES = 256;
	float			  		 	crossFadeCurrentValue;
	unsigned int    	      	frameIndex;  /* Index into sample array. */
	unsigned int    	      	crossFadeframeIndex;  /* Index into sample array. */
	unsigned int          		maxFrameIndex;
	unsigned int 		 		nbChannels;
	SAMPLE      				*recordedSamples;
	bool						waitStart;

	MusicManager* musicManager;
}
soundData;

class MusicManager {
public:
	MusicManager(std::map<std::string, std::string> & tracks,
				 std::vector<std::string>& muteTracks,
				 PaDeviceIndex inputDevice = -1,
				 PaDeviceIndex outputDevice = -1);

	MusicManager(unsigned int timeToRecord);

	virtual ~MusicManager();

	void run();
	void stop();
	void start();
	void pause();
	void play();
	void mute(bool b);

	bool isRunning() const;
	void setMustStop(bool mustStop);

	MultiTracks *getMultiTracks() const;
	void* goToInMs(int millisecPos);

	friend void* musicManagerMainFunction(void* threadArg);

	bool isStarted();
	std::string getCurrentChord() const;

	void fillBufferWithLastInputValues(double* buffer, unsigned int size);

	void saveRecordedData(std::string fileName);

private:

	bool m_isRunning;
	bool m_mustStop;
	std::map<std::string,std::string> m_tracksName;
	boost::thread * m_musicManagerThread;
	MultiTracks *m_multiTracks;

	PaDeviceIndex m_input, m_output;

	PaStreamParameters m_inputParameters;
	PaStreamParameters m_outputParameters;
	PaStream* m_playStream;
	PaStream* m_inputStream;
	PaError m_err;
	soundData m_playData;
	soundData m_recordData;

	std::string m_currentChord;

	void* initAudioDevice(PaDeviceIndex inputDevice = -1, PaDeviceIndex outputDevice = -1);
	void* initAudioInput();
	void* initAudioOutput();
	void* terminateAudioDevice();

	void* changeFrameIndex(int newFrameIndex);

};

#endif /* MUSICMANAGER_H_ */
