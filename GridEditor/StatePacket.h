#ifndef STATEPACKET_H
#define STATEPACKET_H

#include "ChordTableWidget.h"
/**
 * @brief Représente l'état de l'éditeur à un instant donné.
 */
struct StatePacket
{
        StatePacket(ChordTableWidget *m_grid,
                    int m_beginning, int m_bar, int m_end,
                    QString m_artist, QString m_song, QString m_comment,
                    int m_chordsPerBar, int m_timeSignature);

        ChordTableWidget *m_grid; //!< @brief Pointeur vers la grille
        int m_beginning; //!< @brief Temps de début du premier accord
        int m_bar; //!< @brief Temps de fin de la première mesure
        int m_end; //!< @brief Temps de fin du dernier accord
        QString m_artist; //!< @brief Artiste de la piste audio
        QString m_song; //!< @brief Chemin vers la piste audio
        QString m_comment; //!< @brief Commentaire sur le morceau
        int m_chordsPerBar; //!< @brief Nombre d'accords par mesure
        int m_timeSignature; //!< @brief Signature rythmique
};

#endif // STATEPACKET_H
