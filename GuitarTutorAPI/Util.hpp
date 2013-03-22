#ifndef UTIL_HPP
#define UTIL_HPP


#include <QString>
#include <QTime>
int TimeToMsec(QTime t);
int QTimeToSample(QTime t);
QTime MsecToTime(int t);
QTime SampleToQTime(int smp);

QString stringToSub(QString);
#endif // UTIL_HPP
