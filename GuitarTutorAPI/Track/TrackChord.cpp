#include "TrackChord.h"

/**
 * @brief TrackChord::TrackChord
 */
TrackChord::TrackChord()
{
    m_currentChord = 0;
}

/**
 * @brief TrackChord::TrackChord
 * @param name Nom de l'accord
 * @param d Temps de début de l'accord
 * @param rep Nombre de répétitions de l'accord
 * @param previous Pointeur vers l'accord précédent
 * @param next Pointeur vers l'accord suivant
 * @param part Pointeur vers la partie dans laquelle se trouve l'accord
 */
TrackChord::TrackChord(QString name, qreal d, int rep, TrackChord *previous, TrackChord *next, PartTrack *part) {

    m_beginningInMs = d;
    m_repetition = rep;
    m_currentChord = new BasicChord(name);
	m_validated = false;
	m_played = false;
    m_playing= false;

	m_previous = previous;
	m_next = next;
	m_part = part;
}

/**
 * @brief TrackChord::~TrackChord
 */
TrackChord::~TrackChord()
{
    if(m_currentChord != 0)
        delete m_currentChord;
}

/**
 * @brief TrackChord::setRepetition
 * @param newRepetition Nouveau nombre de répétitions
 *
 * Modification du nombre de répétitions de l'accord.
 */
void TrackChord::setRepetition(int newRepetition) {

    m_repetition = newRepetition;
}

/**
 * @brief TrackChord::getRepetition
 * @return Le nombre de répétitions de l'accord.
 */
int TrackChord::getRepetition() {

    return m_repetition;
}

/**
 * @brief TrackChord::getBeginningInMs
 * @return Le temps de début de l'accord.
 */
qreal TrackChord::getBeginningInMs() {

    return m_beginningInMs;
}

/**
 * @brief TrackChord::getChord
 * @return QString accord
 *
 * Permet de Récupérer l'accord sous forme de QString.
 */
QString TrackChord::getChord(){
    if(this == NULL || m_currentChord == 0)
		return "";
    return m_currentChord->toString();
}

/**
 * @brief TrackChord::toString
 * @return Les infos sur l'accord.
 *
 * Fonction utile au débug.
 */
QString TrackChord::toString(){

	QString info = ("Accord = ");
    info.append(m_currentChord->toString());
	info.append(" Duration = ");
    info.append(QString::number(m_beginningInMs, 'e'));
	info.append(" repetition = ");
    info.append(m_repetition);
	return info;
}

/**
 * @brief TrackChord::validate
 * @param b True si l'accord doit etre validé, false sinon
 *
 * Demande la validation ou non de l'accord.
 */
void TrackChord::validate(bool b)
{
	m_validated = b;
}

/**
 * @brief TrackChord::isValidated
 * @return True si la note a été validée, false sinon.
 */
bool TrackChord::isValidated() const
{
	return m_validated;
}

/**
 * @brief TrackChord::setPlayed
 * @param b True si l'accord a été joué, false sinon.
 *
 * Indique si l'accord a déjà été joué.
 */
void TrackChord::setPlayed(bool b)
{
	m_played = b;
}

/**
 * @brief TrackChord::isPlayed
 * @return True si l'accord a déjà été joué, false sinon.
 */
bool TrackChord::isPlayed() const
{
	return m_played;
}

/**
 * @brief TrackChord::setPlaying
 * @param b True si l'accord est celui qui est actuellement joué, false sinon.
 */
void TrackChord::setPlaying(bool b)
{
	m_playing = b;
}

/**
 * @brief TrackChord::isPlaying
 * @return True si l'accord est celui qui est actuellement joué, false sinon.
 */
bool TrackChord::isPlaying() const
{
	return m_playing;
}

/**
 * @brief TrackChord::next
 * @return Un pointeur vers l'accord suivant.
 */
TrackChord* TrackChord::next()
{
	return m_next;
}

/**
 * @brief TrackChord::previous
 * @return Un pointeur vers l'accord précédent.
 */
TrackChord* TrackChord::previous()
{
	return m_previous;
}

/**
 * @brief TrackChord::part
 * @return Un pointeur vers la partie dans laquelle se situe l'accord.
 */
PartTrack* TrackChord::part()
{
	return m_part;
}

/**
 * @brief TrackChord::setNext
 * @param n Pointeur vers l'accord suivant
 *
 * Modifie l'accord suivant.
 */
void TrackChord::setNext(TrackChord* n)
{
	m_next = n;
}

/**
 * @brief TrackChord::setPrevious
 * @param n Pointeur vers l'accord précédent
 *
 * Modifie l'accord précédent.
 */
void TrackChord::setPrevious(TrackChord* n)
{
	m_previous = n;
}

/**
 * @brief TrackChord::setPart
 * @param p Pointeur vers la partie
 *
 * Modifie la partie dans laquelle se situe l'accord.
 */
void TrackChord::setPart(PartTrack* p)
{
	m_part = p;
}
