#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#ifndef WIN32
#include <fmodex/fmod.h>
#else
#include "../fmod/api/inc/fmod.h"
#endif

#include <QString>
#include <QImage>

/**
 * @brief Classe générant les fonctions de base d'un lecteur audio
 */
class MusicPlayer
{
    bool state;
    QString song;
    FMOD_SYSTEM *system;
    FMOD_SOUND *music;
    FMOD_CHANNEL *channel;

	int *samples;
public:
    MusicPlayer();
    MusicPlayer(QString);
    ~MusicPlayer();
    void play();
    void pause();
    void stop();
    bool setSong(QString);
    QString getSong() const;
    bool getState() const;
    unsigned int getPosition() const;
    unsigned int getTotalLength() const;
	unsigned int getTotalLengthInSamples() const;
    void changePosition(unsigned int position);

	void getFullSpectrum(int *tab, unsigned int size);
	void getSpectrum(int begin, int end, int* tab, unsigned int pixelSize);
	void initSamples();

};

#endif // MUSICPLAYER_H
