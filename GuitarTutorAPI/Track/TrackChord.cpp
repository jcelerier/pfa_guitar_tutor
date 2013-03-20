#include "TrackChord.h"
#include <QDebug>

TrackChord::TrackChord()
{
}

TrackChord::TrackChord(QString nom, qreal d, int rep) {

	beginning_in_ms = d;
	repetition = rep;
	currentChord = new BasicChord(nom);
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
