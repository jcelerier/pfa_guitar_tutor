#ifndef UTIL_HPP
#define UTIL_HPP

#include <QTime>
int TimeToMsec(QTime t);
int QTimeToSample(QTime t);
QTime MsecToTime(int t);
QTime SampleToQTime(int smp);
#endif // UTIL_HPP
