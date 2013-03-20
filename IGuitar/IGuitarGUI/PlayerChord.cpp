#include "PlayerChord.h"

/**
 * @brief PlayerChord::getName
 * @return Le nom de l'accord.
 *
 * Getter pour le nom de l'accord.
 */
QString PlayerChord::getName() const
{
    return m_name;
}

/**
 * @brief PlayerChord::getTime
 * @return Le temps de début de l'accord.
 *
 * Getter pour le temps de début de l'accord.
 */
int PlayerChord::getTime() const
{
    return m_time;
}

/**
 * @brief PlayerChord::setName
 * @param n Nouveau nom
 *
 * Setter pour le nom de l'accord.
 */
void PlayerChord::setName(const QString n) {
    m_name = n;
}

/**
 * @brief PlayerChord::setTime
 * @param t Temps de début de l'accord
 *
 * Setter pour le temps de début de l'accord.
 */
void PlayerChord::setTime(const int t) {
    m_time = t;
}

/**
 * @brief PlayerChord::getFullSongItem
 * @return L'item correspondant à l'ensemble des accords sur l'interface.
 *
 * Getter pour l'élément récapitulatif du morceau dans l'interface.
 */
QGraphicsRectItem *PlayerChord::getFullSongItem() const
{
    return m_fullSongItem;
}

/**
 * @brief PlayerChord::setFullSongItem
 * @param r Nouvel élément de l'interface.
 *
 * Setter pour l'élément récapitulatif du morceau dans l'interface.
 */
void PlayerChord::setFullSongItem(QGraphicsRectItem* r)
{
    m_fullSongItem = r;
}

/**
 * @brief PlayerChord::setScrollingChordItem
 * @param r Nouvel élément de l'interface.
 *
 * Setter pour l'élément des accords défilants.
 */
void PlayerChord::setScrollingChordItem(QGraphicsTextItem* r)
{
    m_scrollingChordItem = r;
}

/**
 * @brief PlayerChord::getTrackChord
 * @return Le trackChord correspondant a cet accord.
 *
 * Getter pour l'élément identifiant dans le controler.
 */
TrackChord *PlayerChord::getTrackChord() const
{
    return m_trackChord;
}

/**
 * @brief PlayerChord::setFullSongItem
 * @param r Nouvel élément de l'interface.
 *
 * Setter pour l'élément récapitulatif du morceau dans l'interface.
 */
void PlayerChord::setTrackChord(TrackChord* r)
{
   m_trackChord = r;
}
