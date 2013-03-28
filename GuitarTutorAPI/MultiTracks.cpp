/*
 * MultiTracks.cpp
 *
 *  Created on: 9 mars 2011
 *      Author: raphael
 */

#include "MultiTracks.h"
#include <climits>

/**
 * @brief MultiTracks::MultiTracks
 * @param tracks Liste des tracks
 */
MultiTracks::MultiTracks(QMap<QString, QString>& tracks)
{
	m_bufferSize = INT_MAX;

    QMap<QString, QString>::Iterator it = tracks.begin();

	while (it != tracks.end()) {
        Track currentTrack(it.value());
        m_tracks[it.key()] = currentTrack;

		if (currentTrack.getFramesCount() < m_bufferSize) {
			m_bufferSize = currentTrack.getFramesCount();
		}

		m_nbChannels = currentTrack.getChannelsCount();
		++it;
	}

	m_buffer = new float[m_bufferSize * m_nbChannels];
}

/**
 * @brief MultiTracks::MultiTracks
 * @param silenceTimeInMs Temps de silence entre les tracks
 */
MultiTracks::MultiTracks(unsigned int silenceTimeInMs)
{
	Track currentTrack(silenceTimeInMs);
	m_tracks["all"] = currentTrack;

	m_bufferSize = currentTrack.getFramesCount();
	m_nbChannels = currentTrack.getChannelsCount();

	m_buffer = new float[m_bufferSize * m_nbChannels];
}

/**
 * @brief MultiTracks::~MultiTracks
 */
MultiTracks::~MultiTracks() {
	//delete [] m_buffer;
}

/**
 * @brief MultiTracks::changeTrackMuteState
 * @param trackName Nom de la track
 * @param muteState True si le son doit etre coupé, false sinon
 */
void MultiTracks::changeTrackMuteState(QString trackName, bool muteState)
{
    QMap<QString, Track>::Iterator iter = m_tracks.find(trackName);
	if(iter != m_tracks.end())
	{
		m_tracks[trackName].setMuteState(muteState);
	}
}

/**
 * @brief MultiTracks::isTrackMute
 * @param trackName Nom de la track
 * @return True si la track est en mode silence, false sinon.
 */
bool MultiTracks::isTrackMute(QString trackName)
{
    QMap<QString, Track>::Iterator iter = m_tracks.find(trackName);
	if(iter != m_tracks.end())
	{
		return m_tracks[trackName].isMute();
	}
    return true;
}

/**
 * @brief MultiTracks::getBuffer
 * @return Le buffer utilisé dans MultiTracks
 */
float* MultiTracks::getBuffer() const
{
	return m_buffer;
}

/**
 * @brief MultiTracks::generateMusic
 *
 * Lance la lecture.
 */
void MultiTracks::generateMusic()
{
	for (int i = 0; i < m_bufferSize * m_nbChannels; ++i) {
		m_buffer[i] = 0;
	}

    QMap<QString, Track>::Iterator it = m_tracks.begin();

	while (it != m_tracks.end()) {
        if (!it.value().isMute()) {
			for (int i = 0; i < m_bufferSize * m_nbChannels; ++i) {
                m_buffer[i] += it.value().getBuffer()[i];
			}

		}
		++it;
	}
}

/**
 * @brief MultiTracks::getBufferSize
 * @return La taille du buffer
 */
int MultiTracks::getBufferSize() const
{
	return m_bufferSize;
}

/**
 * @brief MultiTracks::getNbChannels
 * @return Le nombre de canaux utilisés par les tracks.
 */
int MultiTracks::getNbChannels() const
{
	return m_nbChannels;
}





