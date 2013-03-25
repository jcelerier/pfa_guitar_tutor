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

/**
 * @brief PartTrack::getChord
 *
 * Permet de récupérer le nom d'une partie.
 */
QString PartTrack::getPartName(){
	return partName;
}

/**
 * @brief PartTrack::getChord
 *
 * Permet de récupérer la liste des accords correspondant à une partie.
 */
QList<TrackChord*> PartTrack::getTrackChordsList(){
	return listTrackChords;
}

/**
 * @brief PartTrack::incrementCurrentChordRepetition
 *
 * Augmente la valeur du nombre de répétition d'un .
 */
void PartTrack::incrementCurrentChordRepetition()
{
	listTrackChords.last()->setRepetition(listTrackChords.last()->getRepetition() + 1);
}

void PartTrack::setNext(PartTrack* n)
{
	m_next = n;
}

void PartTrack::setPrevious(PartTrack* p)
{
	m_previous = p;
}

PartTrack* PartTrack::next()
{
	return m_next;
}

PartTrack* PartTrack::previous()
{
	return m_previous;
}
