#include "ChordTree.h"

ChordTree::ChordTree() :
    QTreeWidget()
{
    setFixedWidth(200);
    setHeaderLabel(tr("Chord choice"));
    setDisabled(true); //Désactivé par défaut tant que new_button n'a pas été déclenché

    /*Ajout des tonalités*/
    Tonalities* tmp = new Tonalities();
    QStringList tonalities = tmp->getTonalitiesStringList();
    for(int i=0; i<tmp->getTonalitiesNumber(); i++)
        addTopLevelItem(buildChordItem(tonalities[i]));
    delete tmp;
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

    Tuning* tmp = new Tuning();
    QStringList tuning = tmp->getTuningStringList();
    for(int i=0; i<tmp->getTuningNumber(); i++) {
        children[i] = new QTreeWidgetItem();
        children[i]->setText(0, tonality + tuning[i]);
        item->addChild(children[i]);
    }
    delete tmp;
    return item;
}

