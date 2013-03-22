#include "TrackChord.h"
#include <QDebug>

TrackChord::TrackChord()
{
}

TrackChord::TrackChord(QString nom, qreal d, int rep, TrackChord *previous, TrackChord *next, PartTrack *part) {

	beginning_in_ms = d;
	repetition = rep;
	currentChord = new BasicChord(nom);
	m_validated = false;
	m_played = false;
	m_playing=false;

	m_previous = previous;
	m_next = next;
	m_part = part;
}


TrackChord::~TrackChord()
{

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


qreal TrackChord::getBeginningInMs() {

	return beginning_in_ms;
}

/**
 *Permet de Récupérer l'accord sous forme de QString.
 *@return QString : accord
 */
QString TrackChord::getChord(){
	return currentChord->toString();
}

QString TrackChord::toString(){

	QString info = ("Accord = ");
	info.append(currentChord->toString());
	info.append(" Duration = ");
	info.append(QString::number(beginning_in_ms, 'e'));
	info.append(" repetition = ");
	info.append(repetition);
	return info;
}


void TrackChord::validate(bool b)
{
	m_validated = b;
}

bool TrackChord::isValidated() const
{
	return m_validated;
}

void TrackChord::setPlayed(bool b)
{
	m_played = b;
}

bool TrackChord::isPlayed() const
{
	return m_played;
}

void TrackChord::setPlaying(bool b)
{
	m_playing = b;
}

bool TrackChord::isPlaying() const
{
	return m_playing;
}

TrackChord* TrackChord::next()
{
	return m_next;
}

TrackChord* TrackChord::previous()
{
	return m_previous;
}

PartTrack* TrackChord::part()
{
	return m_part;
}
void TrackChord::setNext(TrackChord* n)
{
	m_next = n;
}

void TrackChord::setPrevious(TrackChord* n)
{
	m_previous = n;
}

void TrackChord::setPart(PartTrack* p)
{
	m_part = p;
}
