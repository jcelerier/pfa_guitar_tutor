#include "PlayerChord.h"

/**
 * @brief PlayerChord::getName
 * @return Le nom de l'accord.
 *
 * Getter pour le nom de l'accord.
 */
QString PlayerChord::getName() const
{
    return name;
}

/**
 * @brief PlayerChord::getTime
 * @return Le temps de début de l'accord.
 *
 * Getter pour le temps de début de l'accord.
 */
int PlayerChord::getTime() const
{
    return time;
}

/**
 * @brief PlayerChord::setName
 * @param n Nouveau nom
 *
 * Setter pour le nom de l'accord.
 */
void PlayerChord::setName(const QString n) {
    name = n;
}

/**
 * @brief PlayerChord::setTime
 * @param t Temps de début de l'accord
 *
 * Setter pour le temps de début de l'accord.
 */
void PlayerChord::setTime(const int t) {
    time = t;
}

/**
 * @brief PlayerChord::getFullSongItem
 * @return L'item correspondant à l'ensemble des accords sur l'interface.
 *
 * Getter pour l'élément récapitulatif du morceau dans l'interface.
 */
QGraphicsRectItem *PlayerChord::getFullSongItem() const
{
    return fullSongItem;
}

/**
 * @brief PlayerChord::setFullSongItem
 * @param r Nouvel élément de l'interface.
 *
 * Setter pour l'élément récapitulatif du morceau dans l'interface.
 */
void PlayerChord::setFullSongItem(QGraphicsRectItem* r)
{
    fullSongItem = r;
}

/**
 * @brief PlayerChord::setScrollingChordItem
 * @param r Nouvel élément de l'interface.
 *
 * Setter pour l'élément des accords défilants.
 */
void PlayerChord::setScrollingChordItem(QGraphicsTextItem* r)
{
    scrollingChordItem = r;
}
