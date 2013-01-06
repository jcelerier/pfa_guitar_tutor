#include "PartTrack.h"

/**
 * @brief PartTrack::PartTrack
 *
 * Constructeur par défaut de la classe PartTrack
 */

PartTrack::PartTrack() {

}

/**
 * @brief PartTrack::PartTrack(QString)
 * @param newPartName chaîne de caractère remplaçant l'attribut partName
 *
 * Constructeur surchargé de la classe PartTrack
 */

PartTrack::PartTrack(QString newPartName) {

    partName = newPartName;

}

/**
 * @brief PartTrack::~PartTrack
 *
 * Destructeur de la classe PartTrack
 */


PartTrack::~PartTrack() {

    for(QList<TrackChord*>::Iterator i = listTrackChords.begin(); i != listTrackChords.end() ; i++)
        delete (*i);

}

/**
 * @brief PartTrack::AddChord
 *
 * Fonction de d'ajout d'accord dans l'attribut listTrackChord
 */


void PartTrack::AddChord(TrackChord * c) {

    listTrackChords.append(c);

}

<<<<<<< HEAD
QString PartTrack::getName() const{

    return partName;
}
=======
QString PartTrack::getPartName(){
    return partName;
}

QList<TrackChord*> PartTrack::getListTrackChords(){
    return listTrackChords;
}
>>>>>>> cc378f87033f81e9fee2b178da2ffae6c35f81ae
