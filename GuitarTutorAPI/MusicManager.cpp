/*
 * MusicManager.cpp
 *
 *  Created on: 10 mars 2011
 *      Author: raphael
 */

// quatres actions : start, stop, pause, play
// start : initialisation
// play : lecture (on envoie les données)
// pause : on envoie du silence
// stop : on désinitialise, on revient à 0

#include "MusicManager.h"

#include <boost/thread.hpp>
#include <QDebug>

bool g__output_pause = false;
bool g__output_pause_changed = false;

static int recordCallback( const void *inputBuffer, void *outputBuffer,
						   unsigned long framesPerBuffer,
						   const PaStreamCallbackTimeInfo* timeInfo,
						   PaStreamCallbackFlags statusFlags,
						   void *userData );

static int playCallback( const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData );

void* musicManagerMainFunction(void* threadArg);
/**
 * \fn MusicManager::MusicManager(std::map<std::string, std::string> & tracks, std::vector<std::string>& muteTracks)
 * \brief Constructeur de MusicManager
 *
 * \param tracks Liste des pistes qui seront chargées par l'objet (mapping)
 * \param muteTracks Pistes silencieuses ?
 * \return Rien
 */
MusicManager::MusicManager(std::map<std::string, std::string> & tracks,
						   std::vector<std::string>& muteTracks,
						   PaDeviceIndex inputDevice,
						   PaDeviceIndex outputDevice)
	:m_isRunning(false), m_mustStop(false), m_tracksName(tracks), m_input(inputDevice), m_output(outputDevice)
{
	m_err = paNoError;

	std::cout << "Tracks Loading..." << std::endl;
	m_multiTracks = new MultiTracks(m_tracksName);

	for (unsigned int i = 0; i < muteTracks.size(); ++i) {
		m_multiTracks->changeTrackMuteState(muteTracks[i], true);
	}

	std::cout << "Audio Device Initialization" << std::endl;
	initAudioDevice(m_input, m_output);
	initAudioInput();
	initAudioOutput();

	m_mustStop = false;
	m_isRunning = true;
	m_playData.waitStart = true;
	m_recordData.waitStart = true;

//	run();
}

/**
 * \fn MusicManager::MusicManager(unsigned int timeToRecordInMs)
 * \brief Constructeur de MusicManager (inutilisé)
 *
 * \param timeToRecordInMs Temps à enregistrer ?
 * \return Rien
 */
MusicManager::MusicManager(unsigned int timeToRecordInMs)
:m_isRunning(false), m_mustStop(false)
{
	m_err = paNoError;

	m_multiTracks = new MultiTracks(timeToRecordInMs);

	std::cout << "Audio Device Initialization" << std::endl;
	initAudioDevice(m_input, m_output);
	initAudioInput();
	initAudioOutput();

	m_mustStop = false;
	m_isRunning = true;
	m_playData.waitStart = true;
	m_recordData.waitStart = true;

//	run();
}


MusicManager::~MusicManager()
{
	stop();
	m_musicManagerThread->join();
	delete m_multiTracks;

	qDebug() << "MusicManager properly deleted";
}


/**
 * \fn bool MusicManager::isStarted()
 * \brief Dit si l'enregistrement ou la lecture ont lieu
 *
 * \return Booléen
 */
bool MusicManager::isStarted()
{
	return (!m_playData.waitStart && !m_recordData.waitStart);
}


/**
 * \fn void* MusicManager::initAudioDevice()
 * \brief Initialise portaudio
 *
 * \return Rien
 */
void* MusicManager::initAudioDevice(PaDeviceIndex inputDevice, PaDeviceIndex outputDevice)
{
	Pa_Initialize();

	// cette partie commentée est pour pouvoir changer de device audio pour la lecture et l'écriture, par la suite.
	// cf. audioconfiguration dans IGuitarGUI
	/*qDebug() << inputDevice << Pa_GetDeviceInfo(inputDevice)->name;


	if(inputDevice < 0)
	{
		inputDevice = Pa_GetDefaultInputDevice();
	}
	if(outputDevice < 0)
	{
		outputDevice = Pa_GetDefaultOutputDevice();
	}*/
	inputDevice = Pa_GetDefaultInputDevice();
	outputDevice = Pa_GetDefaultOutputDevice();

	m_inputParameters.device = inputDevice;
	if (m_inputParameters.device == paNoDevice) {
		std::cerr << "Error: No default input device." << std::endl;
		m_isRunning = false;
		return NULL;
	}


	m_inputParameters.channelCount = 2;                    /* stereo input */
	m_inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	m_inputParameters.suggestedLatency = Pa_GetDeviceInfo( m_inputParameters.device )->defaultLowInputLatency;
	m_inputParameters.hostApiSpecificStreamInfo = NULL;


	m_outputParameters.device = outputDevice;
	if (m_outputParameters.device == paNoDevice) {
		std::cerr << "Error: No default output device." << std::endl;

		m_isRunning = false;
		return NULL;
	}
	m_outputParameters.channelCount = 2;                     /* stereo output */
	m_outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
	m_outputParameters.suggestedLatency = Pa_GetDeviceInfo( m_outputParameters.device )->defaultLowOutputLatency;
	m_outputParameters.hostApiSpecificStreamInfo = NULL;

	return NULL;
}

/**
 * \fn void* MusicManager::initAudioInput()
 * \brief Initialise l'entrée audio
 *
 * \return NULL
 */
void* MusicManager::initAudioInput()
{
	m_recordData.frameIndex = 0;

	m_recordData.recordedSamples = new SAMPLE[m_multiTracks->getBufferSize() * 2];
	m_recordData.nbChannels = m_multiTracks->getNbChannels();
	m_recordData.maxFrameIndex = m_multiTracks->getBufferSize();

	m_err = Pa_OpenStream(
			&m_inputStream,
			&m_inputParameters,
			NULL,                  /* &outputParameters, */
			SAMPLE_RATE,
			INPUT_FRAMES_PER_BUFFER,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			recordCallback,
			&m_recordData );

	printf("erreur: %s.\n", Pa_GetErrorText(m_err));
/*
	if( m_inputStream ) {
		Pa_StartStream( m_inputStream );
	}
*/
	return NULL;
}

/**
 * \fn void* MusicManager::initAudioOutput()
 * \brief Initialise la sortie audio
 *
 * \return NULL
 */
void* MusicManager::initAudioOutput()
{
	m_playData.frameIndex = 0;
	m_playData.crossFadeframeIndex = 0;

	m_playData.recordedSamples = m_multiTracks->getBuffer();
	m_playData.nbChannels = m_multiTracks->getNbChannels();
	m_playData.maxFrameIndex = m_multiTracks->getBufferSize();

	m_playData.crossFadeCurrentValue = 0;

	std::cout << std::endl << "=== Now playing. ===" << std::endl;
	m_err = Pa_OpenStream(
			&m_playStream,
			NULL, /* no input */
			&m_outputParameters,
			SAMPLE_RATE,
			OUTPUT_FRAMES_PER_BUFFER,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			playCallback,
			&m_playData );

	m_multiTracks->generateMusic();
/*
	if( m_playStream ) {
		Pa_StartStream( m_playStream );
	}
*/
	return NULL;
}


/**
 * \fn void* MusicManager::terminateAudioDevice()
 * \brief Met fin au streaming
 *
 * \return NULL
 */
void* MusicManager::terminateAudioDevice()
{
	std::cout << "terminate" << std::endl;
	Pa_AbortStream(m_playStream);
	Pa_AbortStream(m_inputStream);
	m_err = Pa_Terminate();
	if( m_playData.recordedSamples )       /* Sure it is NULL or valid. */
		delete m_playData.recordedSamples;
	if( m_recordData.recordedSamples) {
		delete m_recordData.recordedSamples;
	}
	if( m_err != paNoError )
	{
		std::cerr <<  "An error occured while using the portaudio stream" << std::endl;
		std::cerr <<  "Error number: " << m_err << std::endl;
		std::cerr <<  "Error message: " << Pa_GetErrorText( m_err ) << std::endl;
		m_err = 1;          /* Always return 0 or 1, but no other return codes. */
	}

	return NULL;
}




/**
 * \fn MultiTracks *MusicManager::getMultiTracks() const
 * \brief Getter de m_multiTracks
 *
 * \return Pointeur sur m_multiTracks
 */
MultiTracks *MusicManager::getMultiTracks() const
{
	return m_multiTracks;
}

/**
 * \fn void* MusicManager::changeFrameIndex(int newFrameIndex)
 * \brief Positionne la lecture à une nouvelle sample ?
 *
 * \param newFrameIndex Nouvelle sample ou on est positioné
 * \return Pointeur NULL
 */
void* MusicManager::changeFrameIndex(int newFrameIndex)
{
	m_playData.crossFadeframeIndex = m_playData.frameIndex;
	m_playData.frameIndex = newFrameIndex;
	m_playData.crossFadeCurrentValue = 1;

	return NULL;
}

/**
 * \fn void* MusicManager::goToInMs(int millisecPos)
 * \brief Se déplace à une position donnée en ms dans le morceau (je pense qu'il serait plus judicieux d'utiliser le déplacement en samples
 *
 * \param millisecPos Temps en millisecondes ou l'on doit se rendre
 * \return Pointeur NULL
 */
void* MusicManager::goToInMs(int millisecPos)
{
	qDebug() << "MusicManager::goToInMs()";
	if (!isStarted()) {
		//start();
	}

	return changeFrameIndex(SAMPLE_RATE/1000 * millisecPos);
}

/**
 * \fn void MusicManager::fillBufferWithLastInputValues(double* buffer, unsigned int size)
 * \brief D'après le nom, remplit un buffer avec les dernières samples capturées en entrée? (pourquoi ne pas utiliser une classe buffer?)
 *
 * \param buffer Le buffer à remplir
 * \param size Sa taille
 * \return Rien
 */
void MusicManager::fillBufferWithLastInputValues(double* buffer, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i)
	{
		buffer[i] = 0.0;
	}

	int start = m_recordData.frameIndex - NUM_CHANNELS * size;

	if (start < 0)
	{
		return;
	}

	for (unsigned int i = 0; i < size; ++i)
	{
		for (unsigned int j = 0; j < NUM_CHANNELS; ++j)
		{
			buffer[i] += m_recordData.recordedSamples[(NUM_CHANNELS * (i + start)) + j] / NUM_CHANNELS;
		}

		buffer[i] *= NUM_CHANNELS;
	}
}


/**
 * \fn void MusicManager::saveRecordedData(std::string fileName)
 * \brief Méthode qui sauvegarde toutes les données enregistrées
 *
 * \param fileName Nom du fichier sauvegardé
 * \return Rien
 */
void MusicManager::saveRecordedData(std::string fileName)
{
	// On renseigne les paramètres du fichier à créer
	SF_INFO fileInfos;
	fileInfos.channels   = 2;
	fileInfos.samplerate = 44100;
	fileInfos.format     = SF_FORMAT_FLOAT | SF_FORMAT_WAV;

	// On ouvre le fichier en  écriture
	SNDFILE* file = sf_open(fileName.c_str(), SFM_WRITE, &fileInfos);
	if (!file)
		return;

	// Ecriture des échantillons audio
	sf_write_float(file, &m_recordData.recordedSamples[0], m_recordData.frameIndex * m_recordData.nbChannels);

	// Fermeture du fichier
	sf_close(file);

	std::cout << "Save complete on " << fileName << std::endl;
}


/**
 * \fn void MusicManager::run()
 * \brief Lance le thread principal
 *
 * \return Rien
 */

// utilisé pour le premier lancement !
void MusicManager::run()
{
	qDebug() << "MusicManager::run()";
	m_isRunning = true;

	m_musicManagerThread = new boost::thread(&musicManagerMainFunction, this);
//	pthread_create(&m_musicManagerThread, NULL, musicManagerMainFunction, this);
}

/**
 * \fn void MusicManager::stop()
 * \brief Arrete le playback
 *
 * \return Rien
 */
void MusicManager::stop()
{
	qDebug() << "MusicManager::stop()";
	m_mustStop = true;
}

/**
 * \fn void MusicManager::start()
 * \brief Démarre le playback
 *
 * \return Rien
 */
void MusicManager::start()
{
	qDebug() << "MusicManager::start()";
	/*if( m_playStream ) {
		Pa_StartStream( m_playStream );
	}

	if( m_inputStream ) {
		Pa_StartStream( m_inputStream );
	}*/
	m_playData.waitStart = false;
	m_recordData.waitStart = false;
}

/**
 * \fn bool MusicManager::isRunning() const
 * \brief Dit si la lecture ou l'enregistrement sont en cours
 *
 * \return Un booléen correspondant
 */
bool MusicManager::isRunning() const
{
	return m_isRunning;
}

/**
 * \fn void MusicManager::setMustStop(bool m_mustStop)
 * \brief Arrete la lecture / enregistrement si nécessaire
 *
 * \param mustStop Booléen, TRUE si doit s'arreteer
 * \return Rien
 */
void MusicManager::setMustStop(bool mustStop)
{
	this->m_mustStop = mustStop;
}

//////////////////////// fonctions à déplacer (ce sont des callbacks,
//////////////////////// ils sont appelés à chaque fois qu'un paquet audio est dispo
/**
 * \fn static int playCallback( const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData )
 * \brief Fonction appellée pour lire un fichier sur la sortie audio
 *
 * \param inputBuffer Buffer d'entrée (les données du fichier)
 * \param outputBuffer Buffer de sortie (le tampon de la sortie audio)
 * \param framesPerBuffer Taille des buffers
 * \param timeInfo Sample actuelle ? (inutilisé)
 * \param statusFlags Paramètres de Portaudio ? (inutilisé)
 * \param userData En pratique, un soundData
 *
 * \return Un entier (finished, dit si on arrive au bout du buffer ?)
 */
static int playCallback( const void *inputBuffer,
						 void *outputBuffer,
						 unsigned long framesPerBuffer,
						 const PaStreamCallbackTimeInfo* timeInfo,
						 PaStreamCallbackFlags statusFlags,
						 void *userData )
{
	soundData *data = (soundData*)userData;
	SAMPLE *rptr = &data->recordedSamples[data->frameIndex * data->nbChannels];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
	unsigned int i, j;
	int finished;
	unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

	(void) inputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	if (data->waitStart || g__output_pause)
	{
		for( i = 0; i < (framesPerBuffer * NUM_CHANNELS); i++ )
		{
			//	std::cout << i << std::endl;
			*wptr++ = SAMPLE_SILENCE;
		}
		return paContinue;
	}

	if( framesLeft < framesPerBuffer )
	{
		/* final buffer... */
		for( i=0; i<framesLeft; i++ )
		{
			*wptr++ = *rptr++;  /* left */
			if( data->nbChannels == 2 ) *wptr++ = *rptr++;  /* right */
		}
		for( ; i<framesPerBuffer; i++ )
		{
			*wptr++ = 0;  /* left */
			if( data->nbChannels == 2 ) *wptr++ = 0;  /* right */
		}
		data->frameIndex += framesLeft;
		finished = paComplete;
	}
	else
	{
		for( i=0; i<framesPerBuffer; i++ )
		{
			for ( j = 0; j < data->nbChannels; j++) {
				*wptr = (1. - data->crossFadeCurrentValue) * *rptr + (data->crossFadeCurrentValue) * data->recordedSamples[data->crossFadeframeIndex];

				data->crossFadeCurrentValue -= 1. / (float) data->NB_CROSSFADE_FRAMES;

				if (data->crossFadeCurrentValue < 0) { data->crossFadeCurrentValue = 0; }
				if (data->crossFadeCurrentValue != 0) { data->crossFadeframeIndex++; }

				wptr++;
				rptr++;
			}
			//			if( data->nbChannels == 2 ) *wptr++ = *rptr++;  /* right */
		}
		data->frameIndex += framesPerBuffer;
		finished = paContinue;
	}
	return finished;
}

/**
* \fn static int recordCallback( const void *inputBuffer, void *outputBuffer,
	   unsigned long framesPerBuffer,
	   const PaStreamCallbackTimeInfo* timeInfo,
	   PaStreamCallbackFlags statusFlags,
	   void *userData )
* \brief Fonction appellée pour lire un fichier sur la sortie audio
*
* \param inputBuffer Buffer d'entrée (les données du fichier)
* \param outputBuffer Buffer de sortie (le tampon de la sortie audio)
* \param framesPerBuffer Taille des buffers
* \param timeInfo Sample actuelle ? (inutilisé)
* \param statusFlags Paramètres de Portaudio ? (inutilisé)
* \param userData En pratique, un soundData
* \return Un entier, qui dit si on arrive à la fin du buffer ?
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
						   unsigned long framesPerBuffer,
						   const PaStreamCallbackTimeInfo* timeInfo,
						   PaStreamCallbackFlags statusFlags,
						   void *userData )
{
	soundData *data = (soundData*)userData;
	unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;
	if (framesLeft <= 0)
	{
		return paComplete;
	}

	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
	SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	long framesToCalc;
	int finished;

	(void) outputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	if (data->waitStart)
	{
		for( unsigned long i = 0; i < (framesPerBuffer * NUM_CHANNELS); i++ )
		{
			*wptr++ = SAMPLE_SILENCE;
		}
		return paContinue;
	}

	if( framesLeft < framesPerBuffer )
	{
		framesToCalc = framesLeft;
		finished = paComplete;
	}
	else
	{
		framesToCalc = framesPerBuffer;
		finished = paContinue;
	}

	if( inputBuffer == NULL )
	{
		for( long i=0; i<framesToCalc; i++ )
		{
			*wptr++ = SAMPLE_SILENCE;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
		}
	}
	else
	{
		for( long i=0; i<framesToCalc; i++ )
		{
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
	}
	data->frameIndex += framesToCalc;

	return finished;
}



/**
 * \fn void* musicManagerMainFunction(void* threadArg)
 * \brief Thread principal (lancé par run() appelé à la fin du constructeur de MusicManager). Ne fait PAS partie de la classe
 *
 * \param threadArg Instance de MusicManager
 * \return Rien
 */
void* musicManagerMainFunction(void* threadArg)
{
	qDebug()<< "musicManagerMainFunction";
	MusicManager* musicManager = (MusicManager*) threadArg;
/*
	if( musicManager->m_playStream ) {
		Pa_StartStream( musicManager->m_playStream );
	}

	if( musicManager->m_inputStream ) {
		Pa_StartStream( musicManager->m_inputStream );
	}
*/
	while (!musicManager->m_mustStop)
		//	&& (Pa_IsStreamActive(musicManager->m_playStream)))
	{
		Pa_Sleep(100);
		if(g__output_pause_changed && g__output_pause )
		{
			g__output_pause_changed = false;
			Pa_StopStream(musicManager->m_playStream);
			Pa_StopStream(musicManager->m_inputStream);
		}
		else if(g__output_pause_changed && !g__output_pause)
		{
			g__output_pause_changed = false;
			Pa_StartStream(musicManager->m_playStream);
			Pa_StartStream(musicManager->m_inputStream);
		}

	}

	//musicManager->terminateAudioDevice();

	Pa_StopStream(musicManager->m_playStream);
	Pa_StopStream(musicManager->m_inputStream);

	musicManager->m_playData.waitStart = true;
	musicManager->m_recordData.waitStart = true;

	musicManager->m_isRunning = false;
	qDebug() << "leaving musicManagerMainFunction";
	return NULL;
}

void MusicManager::pause()
{
	g__output_pause = true;
	g__output_pause_changed = true;
}
void MusicManager::play()
{
	g__output_pause = false;
	g__output_pause_changed = true;
}

