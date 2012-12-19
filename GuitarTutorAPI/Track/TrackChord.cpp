#include "TrackChord.h"

TrackChord::TrackChord()
{
}

TrackChord::TrackChord(QString nom, int t, int rep) {

time = t;
repetition = rep;

currentChord = new Chord(nom);

}


TrackChord::~TrackChord() {

}

/**
 * @brief TrackChord::setRepetition
 * @param newRepetition entier remplaçant l'attribut repetition
 *
 * Fonction de modification du champ repetition
 */


void TrackChord::setRepetition(int newRepetition) {

    repetition = newRepetition;

}

/**
 * @brief TrackChord::getRepetition
 *
 * Fonction de récupération du champ repetition
 */


int TrackChord::getRepetition() {

    return repetition;

}

