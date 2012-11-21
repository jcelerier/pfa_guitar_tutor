#ifndef CHORDTREE_H
#define CHORDTREE_H

#include <QTreeWidget>

class ChordTree : public QTreeWidget
{
    Q_OBJECT
public:
    ChordTree();
    QTreeWidgetItem *buildChordItem(const QString tonality);
    
};

#endif // CHORDTREE_H
