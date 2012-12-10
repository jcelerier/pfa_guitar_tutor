#include "Tonalities.h"

/**
 * @brief Tonalities::Tonalities
 *
 * Constructeur par défaut
 */
Tonalities::Tonalities()
{
    tonalitiesStringList = (QStringList() << "C" << "C#" << "Db" << "D" << "D#" << "Eb" << "E" << "F"
                            << "F#" << "Gb" << "G" << "G#" << "Ab" << "A" << "A#" << "Bb" << "B");
    tonalitiesNumber = tonalitiesStringList.size();
}

/**
 * @brief Tonalities::getTonalitiesStringList
 * @return La liste des tonalités.
 *
 * Renvoit la liste des tonalités possibles sous forme de liste de chaînes de caractères.
 */
QStringList Tonalities::getTonalitiesStringList()
{
    return tonalitiesStringList;
}

/**
 * @brief Tonalities::getTonalitiesNumber
 * @return Le nombre de tonalités.
 *
 * Renvoit le nombre de tonalités possibles.
 */
int Tonalities::getTonalitiesNumber()
{
    return tonalitiesNumber;
}
