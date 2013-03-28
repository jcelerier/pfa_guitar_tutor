/*
 * Track.cpp
 *
 *  Created on: 9 mars 2011
 *      Author: raphael
 */

#include "Track.h"
#include <QVector>

#include <cmath>
#include <QFile>

#include <QDebug>
#define APIC_HEADER_LENGTH 14

/**
 * @brief Track::Track
 *
 * Constructeur par défaut
 */
Track::Track()
{

}


/**
 * @brief Track::Track
 * @param fileName
 *
 * Constructeur de Track: prend en paramètre le nom du fichier, le met à jour dans Track, m_isMute reçoi false. Ensuite, charge le fichier.
 */
Track::Track(QString fileName) {
	m_fileName = fileName;
	m_isMute = false;

	load();
}


/**
 * @brief Track::Track
 * @param silenceTrackTimeInMs La durée de la piste durant laquelle il n'y a pas de son.
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

/**
 * @brief Track::~Track
 */
Track::~Track()
{
	//delete[] m_buffer;
}

/**
 * @brief Track::getBuffer
 *
 * Retourne le buffer de la piste.
 */
float *Track::getBuffer() const
{
	return m_buffer;
}

/**
 * @brief Track::getChannelsCount
 *
 * Retourne le nombre de cannaux utilisés par la piste.
 */
int Track::getChannelsCount() const
{
	return m_channelsCount;
}
/**
 * @brief Track::getFramesCount
 *
 * Retourne le nombre de frames de la piste.
 */

int Track::getFramesCount() const
{
	return m_framesCount;
}

/**
 * @brief Track::setFramesCount
 * @param m_framesCount Le nombre de frames
 */
void Track::setFramesCount(int m_framesCount)
{
	this->m_framesCount = m_framesCount;
}

/**
 * @brief Track::load
 *
 * Charge un fichier audio en mémoire.
 */
int Track::load()
{
	double normalization_factor = pow(2, 16);

	//Init FMOD
	FMOD_SYSTEM *system;
	FMOD_SOUND *music;
	FMOD_System_Create(&system);
	FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);

	FMOD_RESULT result = FMOD_System_CreateSound(system, m_fileName.toStdString().c_str(), FMOD_SOFTWARE | FMOD_2D |  FMOD_CREATESAMPLE, 0, &music);

	if (result != FMOD_OK)
	{
		qDebug() << "Error while opening the file: " << m_fileName;
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
	//Récupération de la pochette de l'album dans les tags
	remove("albumcover.jpg");
	FMOD_TAG tag;
	result = FMOD_Sound_GetTag(music, "APIC", 0, &tag);
	if(result == FMOD_OK && tag.datalen > 0) {
		QFile file("albumcover.jpg");
		if(file.open(QIODevice::WriteOnly)) {
			QByteArray tmp = QByteArray((char*) tag.data, tag.datalen);
			tmp.remove(0,APIC_HEADER_LENGTH);
			file.write(tmp, tag.datalen-APIC_HEADER_LENGTH);
		}
		file.close();
	}

	FMOD_Sound_Unlock(music, pointer1, pointer2, length1, length2);

	//close the file
	FMOD_Sound_Release(music);
	FMOD_System_Close(system);
	FMOD_System_Release(system);
	return 0;
}

/**
 * @brief Track::isMute
 *
 * Retourne true sur la piste est actuellement en mode sourdine, false sinon.
 */
bool Track::isMute()
{
	return m_isMute;
}

/**
 * @brief Track::setMuteState
 * @param isMute Vrai si la piste doit etre mise en sourdine, faux sinon
 *
 * Met ou enlève le mode sourdine de la piste courante.
 */
void Track::setMuteState(bool isMute)
{
	m_isMute = isMute;
}
