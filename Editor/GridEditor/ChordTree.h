#ifndef CHORDTREE_H
#define CHORDTREE_H

#include <QtWidgets/QTreeWidget>
#include <QString>
#include "Tonalities.h"
#include "Enrichment.h"

/**
 * @brief Arbre de sélection d'accords
 */
class ChordTree : public QTreeWidget
{
    Q_OBJECT
public:
    ChordTree();
    QTreeWidgetItem *buildChordItem(const QString tonality);
    
};

#endif // CHORDTREE_H
