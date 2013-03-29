#ifndef UTIL_HPP
#define UTIL_HPP

/**
 * @brief Cette classe permet d'effectuer les convertions entre les millisecondes, les samples et
 * les objets de type QTime. Offre aussi une fonction permettant de formater un accord pour l'affichage.
 */

#include <QString>
#include <QTime>
int TimeToMsec(QTime t);
int QTimeToSample(QTime t);
QTime MsecToTime(int t);
QTime SampleToQTime(int smp);

QString stringToSub(QString);
#endif // UTIL_HPP
