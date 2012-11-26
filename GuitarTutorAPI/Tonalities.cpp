#include "Tonalities.h"

Tonalities::Tonalities()
{
    tonalitiesStringList = (QStringList() << "C" << "C#" << "Db" << "D" << "D#" << "Eb" << "E" << "F"
                            << "F#" << "Gb" << "G" << "G#" << "Ab" << "A" << "A#" << "Bb" << "B");
    tonalitiesNumber = tonalitiesStringList.size();
}

QStringList Tonalities::getTonalitiesStringList()
{
    return tonalitiesStringList;
}

int Tonalities::getTonalitiesNumber()
{
    return tonalitiesNumber;
}
