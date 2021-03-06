#include "Util.hpp"

/**
 * @brief TimeToMsec
 * @param t QTime à convertir
 * @return Le temps en millisecondes
 */
int TimeToMsec(QTime t)
{
	return t.minute() * 60000 + t.second() * 1000 + t.msec();
}

/**
 * @brief MsecToTime
 * @param t Millisecondes à convertir
 * @return QTime correspondant
 */
QTime MsecToTime(int t)
{
	int m = t / 60000;
	int s = (t - m * 60000) / 1000;
	int ms = t - m * 60000 - s * 1000;
	return QTime(0, m, s, ms);
}

/**
 * @brief QTimeToSample
 * @param t un temps au format QTime
 * @return un entier qui correspond à un nombre de samples à 44.1 khZ
 */
int QTimeToSample(QTime t)
{
	return (t.msec() + t.second() * 1000 + t.minute() * 60000) * 44.1;
	//ugly, poller la sample rate de FMOD / portaudio / système....
}

/**
 * @brief SampleToQTime
 * @param smp Le taux d'échantillonage
 * @return Le QTime associé.
 *
 * Conversion du taux d'échantillonage en QTime.
 */
QTime SampleToQTime(int smp)
{
	return MsecToTime(smp / 44.1);
}

/**
 * @brief stringToSub
 * @param str L'accord à formater
 * @return L'accord formaté pour l'affichage.
 *
 * Formate un accord sous forme de chaine de caractère pour l'affichage sur l'interface.
 */
QString stringToSub(QString str) {
    return str[0]+"<sub>"+str.mid(1)+"</sub>";
}
