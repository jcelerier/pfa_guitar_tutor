#ifndef TONALITIES_H
#define TONALITIES_H

#include <QStringList>
#include "Tonality.h"
#include "GuitarTutorAPI_global.h"

#define NUM_TONALITIES 7

/**
 * @brief Objet représentant la liste des différentes tonalités possibles
 */
class Tonalities
{
    QStringList tonalitiesStringList;
    int tonalitiesNumber;

public:
    Tonalities();
    QStringList getTonalitiesStringList();
    int getTonalitiesNumber();
};

#endif // TONALITIES_H
