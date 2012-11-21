#ifndef TONALITIES_H
#define TONALITIES_H

#include <QStringList>
#include "GuitarTutorAPI_global.h"

class GUITARTUTORAPISHARED_EXPORT Tonalities
{
    QStringList tonalitiesStringList;
    int tonalitiesNumber;
public:
    Tonalities();
    QStringList getTonalitiesStringList();
    int getTonalitiesNumber();
};

#endif // TONALITIES_H
