/*
 * MusicManager.h
 *
 *  Created on: 10 mars 2011
 *      Author: raphael
 */

#ifndef MusicManager_H_
#define MusicManager_H_

#include <map>
#include <string>
#include <pthread.h>
#include <sndfile.h>

#include "MultiTracks.h"
#include "portaudio/portaudio.h"
#include <vector>

extern "C" {
#include "chord/chord.h"
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
}
soundData;

class MusicManager {
public:
	MusicManager(std::map<std::string, std::string> & tracks, std::vector<std::string>& muteTracks);

	MusicManager(unsigned int timeToRecord);

	virtual ~MusicManager();

	void run();
	void stop();
	void start();

	bool isRunning() const;
    void setMustStop(bool m_mustStop);

    MultiTracks *getMultiTracks() const;
    void* goToInMs(int millisecPos);

	friend void* MusicManagerMainFunction(void* threadArg);

	bool isStarted();
    std::string getCurrentChord() const;

    void fillBufferWithLastInputValues(double* buffer, unsigned int size);

    void saveRecordedData(std::string fileName);

    void sendOutputVect(std::vector<std::string> &outputVect );
    void sendInputVect(std::vector<std::string> &inputVect );

private:
    bool m_isRunning;
    bool m_mustStop;
    std::map<std::string,std::string> m_tracksName;
	pthread_t m_MusicManagerThread;
    MultiTracks *m_multiTracks;
    PaStreamParameters m_inputParameters;
	PaStreamParameters m_outputParameters;
	PaStream* m_playStream;
	PaStream* m_inputStream;
	PaError m_err;
	soundData m_playData;
	soundData m_recordData;

	std::string m_currentChord;

	void* initAudioDevice();
	void* initAudioInput();
	void* initAudioOutput();
	void* terminateAudioDevice();

	void* changeFrameIndex(int newFrameIndex);

};

#endif /* MusicManager_H_ */
