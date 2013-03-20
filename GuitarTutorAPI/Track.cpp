/*
 * Track.cpp
 *
 *  Created on: 9 mars 2011
 *      Author: raphael
 */

#include "Track.h"
#include <vector>
#include <QDebug>
#include <cmath>

/**
 *
 * @brief Track::Track
 *
 * Constructeur par défaut
 */
Track::Track()
{

}


/**
 *
 * @brief Track::Track
 *
 * @param fileName
 *
 * Constructeur de Track: prend en paramètre le nom du fichier, le met à jour dans Track, m_isMute reçoi false. Ensuite, charge le fichier.
 */
Track::Track(std::string fileName) {
	m_fileName = fileName;
	m_isMute = false;

	load();
}


/**
 *
 * @brief Track::Track
 *
 * @param silenceTrackTimeInMs
 *
 * Constructeur de Track.
 */
Track::Track(unsigned int silenceTrackTimeInMs)
{
	m_channelsCount = NUM_CHANNELS;
	m_framesCount = (silenceTrackTimeInMs/1000.0) * SAMPLE_RATE;

	m_buffer = new float[(unsigned int) (m_framesCount * m_channelsCount)];

	for (unsigned int i = 0; i < (unsigned int) (m_framesCount * m_channelsCount); ++i)
	{
		m_buffer[i] = 0.0;
	}
}

Track::~Track()
{
	//delete[] m_buffer;
}

/**
 *
 *@brief Track::getBuffer
 *
 *Retourne le buffer de Track
 */
float *Track::getBuffer() const
{
	return m_buffer;
}

/**
 *
 *@brief Track::getChannelsCount
 *
 *Retourne m_channelsCount de Track
 */
int Track::getChannelsCount() const
{
	return m_channelsCount;
}
/**
 *
 *@brief Track::getFramesCount
 *
 *Retourne m_framesCount de Track
 */

int Track::getFramesCount() const
{
	return m_framesCount;
}

/**
 *
 *@brief Track::setFramesCount
 *
 *@param m_framesCount
 *
 *Met à jour le champ m_framesCount avec la valeur passée en paramètre
 */
void Track::setFramesCount(int m_framesCount)
{
	this->m_framesCount = m_framesCount;
}

/**
 *
 *@brief Track::load
 *
 *
 *Charge le fichier
 */
int Track::load()
{
	double normalization_factor = pow(2, 16);

	//Init FMOD
	FMOD_SYSTEM *system;
	FMOD_SOUND *music;
	FMOD_System_Create(&system);
	FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);

	FMOD_RESULT result = FMOD_System_CreateSound(system, m_fileName.c_str(), FMOD_SOFTWARE | FMOD_2D |  FMOD_CREATESAMPLE, 0, &music);

	if (result != FMOD_OK)
	{
		qDebug() << m_fileName.c_str();
		return false;
	}

	//test the samplerate
	FMOD_Sound_GetFormat(music, 0, 0, &m_channelsCount, 0); //OK

	//frames number
	FMOD_Sound_GetLength(music, &m_framesCount, FMOD_TIMEUNIT_PCM); //OK

	m_buffer = new float[2*m_framesCount];

	void * pointer1 = 0;
	void * pointer2 = 0;
	unsigned int length1;
	unsigned int length2;
	result = FMOD_Sound_Lock(music, 0, m_framesCount*2, &pointer1, &pointer2, &length1, &length2);

	unsigned int i = 0, j = 0;
	while( i <  m_channelsCount * m_framesCount )
	{
		m_buffer[i] = (((int*)pointer1)[j]>>16) / normalization_factor;
		i++;

		m_buffer[i] = ((((int*)pointer1)[j]<<16)>>16) / normalization_factor;
		i++;
		j++;
	}

	FMOD_Sound_Unlock(music, pointer1, pointer2, length1, length2);

	//close the file
	FMOD_Sound_Release(music);
	FMOD_System_Close(system);
	FMOD_System_Release(system);
	return 0;
}

/**
 *@brief Track::isMute
 *
 *
 *Retourne m_isMute de Track
 */
bool Track::isMute()
{
	return m_isMute;
}

/**
 *@brief Track::setMuteState
 *
 *@param isMute
 *
 *Met à jour le champ m_isMute avec la valeur passée en paramètre.
 */
void Track::setMuteState(bool isMute)
{
	m_isMute = isMute;
}
