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

#include "MultiTracks.h"
#include <portaudio.h>

#if defined(__MINGW32__) || defined(__linux__) || defined(TARGET_OS_MAC)
#include <pthread.h>
#endif
#if defined(_WIN32) &&! defined(__MINGW32__)
#include <Windows.h>
#endif


extern "C" {
#include "chord/chord_c.h"
}


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


#if defined(__MINGW32__) || defined(__linux__) || defined(TARGET_OS_MAC)
	pthread_t m_musicManagerThread;
#endif
#if defined(_WIN32) &&! defined(__MINGW32__)
	HANDLE m_musicManagerThread;
#endif

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
