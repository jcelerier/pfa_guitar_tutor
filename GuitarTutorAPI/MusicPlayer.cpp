#include "MusicPlayer.h"
#include <iostream>

/**
 * @brief MusicPlayer::MusicPlayer
 *
 * Constructeur par défaut.
 */
MusicPlayer::MusicPlayer()
{
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    state = false;
}

/**
 * @brief MusicPlayer::MusicPlayer
 * @param song Chemin vers un fichier audio
 *
 * Constructeur avec initialisation du fichier audio à jouer.
 */
MusicPlayer::MusicPlayer(QString song)
{
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    state = setSong(song);
}

/**
 * @brief MusicPlayer::~MusicPlayer
 */
MusicPlayer::~MusicPlayer()
{
    FMOD_Sound_Release(music);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}

/**
 * @brief MusicPlayer::play
 *
 * Lecture du fichier audio.
 */
void MusicPlayer::play()
{
    if(state) {
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, music, 0, &channel);
        FMOD_Channel_SetPaused(channel, 0);
    }
}

/**
 * @brief MusicPlayer::pause
 *
 * Mise en pause du fichier audio.
 */
void MusicPlayer::pause()
{
    if(state) {
        FMOD_BOOL isPaused;
        FMOD_Channel_GetPaused(channel, &isPaused);

        if (isPaused)
            FMOD_Channel_SetPaused(channel, 0);
        else
            FMOD_Channel_SetPaused(channel, 1);
    }
}

/**
 * @brief MusicPlayer::stop
 *
 * Arrêt de la lecture du fichier audio.
 */
void MusicPlayer::stop()
{
    if(state) {
        state = false;
        FMOD_Channel_Stop(channel);
    }
}

/**
 * @brief MusicPlayer::setSong
 * @param song Chemin vers le fichier audio à lire
 * @return Vrai si et seulement si le fichier audio a pu permettre l'initialisation du lecteur.
 *
 * Charge un morceau à jouer.
 */
bool MusicPlayer::setSong(QString song)
{
    const char* tmp = song.toStdString().c_str();
    FMOD_RESULT result = FMOD_System_CreateSound(system, tmp, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &music);

    if (result != FMOD_OK) {
        state = false;
        return false;
    }
    this->song = song;
    state = true;
    return true;
}

/**
 * @brief MusicPlayer::getSong
 * @return Le chemin vers le fichier audio en cours de lecture.
 *
 * Retourne le chemin vers le fichier audio courant.
 */
QString MusicPlayer::getSong() const
{
    return song;
}

/**
 * @brief MusicPlayer::getState
 * @return Vrai si et seulement si un fichier audio est actuellement attribué au lecteur.
 *
 * Indique si le lecteur est actuellement en cours de lecture.
 */
bool MusicPlayer::getState() const
{
    return state;
}

/**
 * @brief MusicPlayer::getPosition
 * @return La position en millisecondes dans le morceau courant.
 *
 * Indique la position dans le morceau courant.
 */
unsigned int MusicPlayer::getPosition() const
{
    if(state) {
        unsigned int tmp;
        if(FMOD_Channel_GetPosition(channel, &tmp, FMOD_TIMEUNIT_MS) == FMOD_OK)
            return tmp;
    }
    return 0;
}

/**
 * @brief MusicPlayer::getTotalLength
 * @return La durée totale du morceau en millisecondes, ou 0 en cas d'erreur.
 *
 * Indique la durée totale du morceau courant.
 */
unsigned int MusicPlayer::getTotalLength() const
{
    if(state) {
        unsigned int tmp = 0;
        if(FMOD_Sound_GetLength(music, &tmp, FMOD_TIMEUNIT_MS) == FMOD_OK)
            return tmp;
    }
    return 0;
}

/**
 * @brief MusicPlayer::changePosition
 * @param position Nouvelle position en millisecondes.
 *
 * Lit le morceau courant à partir de la nouvelle position.
 */
void MusicPlayer::changePosition(unsigned int position)
{
    if(state)
        FMOD_Channel_SetPosition(channel, position, FMOD_TIMEUNIT_MS);
}
