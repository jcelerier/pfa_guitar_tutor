#ifndef TUNING_H
#define TUNING_H

#include <QStringList>
#include "GuitarTutorAPI_global.h"

class GUITARTUTORAPISHARED_EXPORT Tuning
{
    QStringList tuningStringList;
    int tuningNumber;
public:
    Tuning();
    QStringList getTuningStringList();
    int getTuningNumber();
};

#endif // TUNING_H
