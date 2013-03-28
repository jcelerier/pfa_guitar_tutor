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

	m_partName = newPartName;
}

/**
 * @brief PartTrack::PartTrack
 * @param newPartName Nom de la partie
 * @param newListChords Liste des accords de la partie
 *
 * Constructeur surchargé pour une initialisation complète de PartTrack.
 */
PartTrack::PartTrack(QString newPartName, QList<TrackChord*> newListChords){
	m_partName = newPartName;
	m_listTrackChords = newListChords;
}

/**
 * @brief PartTrack::~PartTrack
 *
 * Destructeur de la classe PartTrack
 */
PartTrack::~PartTrack() {

	for(QList<TrackChord*>::Iterator i = m_listTrackChords.begin(); i != m_listTrackChords.end() ; i++)
		delete (*i);

}

/**
 * @brief PartTrack::AddChord
 *
 * Fonction de d'ajout d'accord dans l'attribut listTrackChord
 */
void PartTrack::AddChord(TrackChord * c)
{
	m_listTrackChords.append(c);
}

/**
 * @brief PartTrack::getChord
 *
 * Permet de récupérer le nom d'une partie.
 */
QString PartTrack::getPartName(){
	return m_partName;
}

/**
 * @brief PartTrack::getChord
 *
 * Permet de récupérer la liste des accords correspondant à une partie.
 */
QList<TrackChord*> PartTrack::getTrackChordsList(){
	return m_listTrackChords;
}

/**
 * @brief PartTrack::incrementCurrentChordRepetition
 *
 * Augmente la valeur du nombre de répétition d'un .
 */
void PartTrack::incrementCurrentChordRepetition()
{
	m_listTrackChords.last()->setRepetition(m_listTrackChords.last()->getRepetition() + 1);
}

/**
 * @brief PartTrack::setNext
 * @param n Pointeur vers la partie suivante
 *
 * Chaine la partie courante avec la partie suivante.
 */
void PartTrack::setNext(PartTrack* n)
{
	m_next = n;
}

/**
 * @brief PartTrack::setPrevious
 * @param p Pointeur vers la partie suivante
 *
 * Chaine la partie courante avec la partie précédente.
 */
void PartTrack::setPrevious(PartTrack* p)
{
	m_previous = p;
}

/**
 * @brief PartTrack::next
 * @return Un pointeur vers la partie suivante.
 *
 * Retourne la partie suivant la partie courante.
 */
PartTrack* PartTrack::next()
{
	return m_next;
}

/**
 * @brief PartTrack::previous
 * @return Un pointeur vers la partie précédente.
 *
 * Retourne la partie précédent la partie courante.
 */
PartTrack* PartTrack::previous()
{
	return m_previous;
}
