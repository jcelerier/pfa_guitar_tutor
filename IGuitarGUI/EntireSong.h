#ifndef ENTIRESONG_H
#define ENTIRESONG_H

#include <QGraphicsItem>
#include <Track/LogicalTrack.h>

class Controler;

/**
 * @brief Objet affichant tous les accords de la chanson a droite de l'ecran ainsi que leur evolution
 * Cette classe est rafraichie a chaque changement d'accord
 */

class EntireSong : public QGraphicsItem
{
public:
	EntireSong(QGraphicsItem *parent = 0);
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget* widget= 0);
    void nextChord();
    void setCurrentChord(TrackChord *tc);
    void load(LogicalTrack*);

private:

    void paintChord(TrackChord*);   // Change la couleur d'un accord en fonction de son etat
    TrackChord* m_currentChord;     // Pointeur vers l'accord courant

    Controler* m_controler;         // Lien vers le controleur

    QMap<TrackChord*, QGraphicsRectItem*> m_gMap;   // Lien entre les accords et leurs case a l'ecran
    LogicalTrack* m_track;          // Chanson actuellement jouee
    bool m_loaded;                  // Permet de savoir si la chanson a ete chargee dans cet objet
    QGraphicsItemGroup* m_container;    // Regroupe tous les accords pour les manipuler par lot
};

#endif // ENTIRESONG_H
