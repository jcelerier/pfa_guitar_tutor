#include "Util.hpp"

/**
 * @brief TimeToMsec
 * @param t QTime à convertir
 * @return Le temps en millisecondes
 *
 * Convertit un QTime en millisecondes
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
