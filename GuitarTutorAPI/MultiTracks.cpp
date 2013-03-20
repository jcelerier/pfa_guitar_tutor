/*
 * MultiTracks.cpp
 *
 *  Created on: 9 mars 2011
 *      Author: raphael
 */

#include "MultiTracks.h"
#include "limits.h"

MultiTracks::MultiTracks(std::map<std::string, std::string>& tracks)
{
	m_bufferSize = INT_MAX;

	std::map<std::string, std::string>::iterator it = tracks.begin();

	while (it != tracks.end()) {
		Track currentTrack(it->second);
		m_tracks[it->first] = currentTrack;

		if (currentTrack.getFramesCount() < m_bufferSize) {
			m_bufferSize = currentTrack.getFramesCount();
		}

		m_nbChannels = currentTrack.getChannelsCount();
		++it;
	}

	m_buffer = new float[m_bufferSize * m_nbChannels];
}

MultiTracks::MultiTracks(unsigned int silenceTimeInMs)
{
	Track currentTrack(silenceTimeInMs);
	m_tracks["all"] = currentTrack;

	m_bufferSize = currentTrack.getFramesCount();
	m_nbChannels = currentTrack.getChannelsCount();

	m_buffer = new float[m_bufferSize * m_nbChannels];
}

MultiTracks::~MultiTracks() {
	//delete [] m_buffer;
}

void MultiTracks::changeTrackMuteState(std::string trackName, bool muteState)
{
	std::map<std::string, Track>::iterator iter = m_tracks.find(trackName);
	if(iter != m_tracks.end())
	{
		m_tracks[trackName].setMuteState(muteState);
	}
}

bool MultiTracks::isTrackMute(std::string trackName)
{
	std::map<std::string, Track>::iterator iter = m_tracks.find(trackName);
	if(iter != m_tracks.end())
	{
		return m_tracks[trackName].isMute();
	}
}

float* MultiTracks::getBuffer() const
{
	return m_buffer;
}

void MultiTracks::generateMusic()
{
	for (int i = 0; i < m_bufferSize * m_nbChannels; ++i) {
		m_buffer[i] = 0;
	}

	std::map<std::string, Track>::iterator it = m_tracks.begin();

	while (it != m_tracks.end()) {
		if (!it->second.isMute()) {
			for (int i = 0; i < m_bufferSize * m_nbChannels; ++i) {
				m_buffer[i] += it->second.getBuffer()[i];
			}

		}
		++it;
	}
}

int MultiTracks::getBufferSize() const
{
	return m_bufferSize;
}

int MultiTracks::getNbChannels() const
{
	return m_nbChannels;
}





