#include "Tuning.h"

Tuning::Tuning()
{
    tuningStringList = (QStringList() << "" << "m" << "aug" << "dim" << "sus4" << "sus2");
    tuningNumber = tuningStringList.size();
}

QStringList Tuning::getTuningStringList()
{
    return tuningStringList;
}

int Tuning::getTuningNumber()
{
    return tuningNumber;
}
