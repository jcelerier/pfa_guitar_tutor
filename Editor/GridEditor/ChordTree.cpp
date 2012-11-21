#include "ChordTree.h"

ChordTree::ChordTree() :
    QTreeWidget()
{
    setFixedWidth(200);
    setHeaderLabel(tr("Chord choice"));
    setDisabled(true); //Désactivé par défaut tant que new_button n'a pas été déclenché

    /*Ajout des cordes*/
    //TODO: tonalities est à prendre de la bibliothèque
    QString tonalities[17] = {"C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G", "G#", "Ab", "A", "A#", "Bb", "B"};
    for(int i=0; i<17; i++)
        addTopLevelItem(buildChordItem(tonalities[i]));
}

/**
 * @brief ChordTree::buildChord
 * @param base_name Tonalité à rajouter à la liste des accords.
 * @return L'élément à rajouter à la liste des accords.
 *
 * Cette fonction crée un objet contenant les différentes possibilités d'accords à partir d'une tonalité donnée: mineur, majeur,...
 */
QTreeWidgetItem* ChordTree::buildChordItem(const QString tonality) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, tonality);

    QTreeWidgetItem* children[6];
    //TODO: tuning est à prendre de la bibliothèque
    QString tuning[6] = {"", "m", "aug", "dim", "sus4", "sus2"};
    for(int i=0; i<6; i++) {
        children[i] = new QTreeWidgetItem();
        children[i]->setText(0, tonality + tuning[i]);
        item->addChild(children[i]);
    }
    return item;
}

