#ifndef CHORDTREE_H
#define CHORDTREE_H

#include <QTreeWidget>
#include "GuitarTutorAPI_global.h"
#include "GuitarTutorAPI.h"
#include "Tonalities.h"
#include "Enrichment.h"

class ChordTree : public QTreeWidget
{
    Q_OBJECT
public:
    ChordTree();
    QTreeWidgetItem *buildChordItem(const QString tonality);
    
};

#endif // CHORDTREE_H
