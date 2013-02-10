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

PartTrack::PartTrack(QString newPartName, QList<TrackChord*> newListChords){
    partName = newPartName;
    listTrackChords = newListChords;
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


void PartTrack::AddChord(TrackChord * c)
{
    listTrackChords.append(c);
}

QString PartTrack::getPartName(){
    return partName;
}

QList<TrackChord*> PartTrack::getTrackChordsList(){
    return listTrackChords;
}

void PartTrack::incrementCurrentChordRepetition()
{
	listTrackChords.last()->setRepetition(listTrackChords.last()->getRepetition() + 1);
}
