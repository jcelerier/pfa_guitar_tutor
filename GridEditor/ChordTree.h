#ifndef CHORDTREE_H
#define CHORDTREE_H

#include <QtWidgets/QTreeWidget>
#include "Tonality.h"
#include "Enrichment.h"


/**
 * @brief Arbre de sélection d'accords
 */
class ChordTree : public QTreeWidget
{
		Q_OBJECT

	public:
		ChordTree();
		QTreeWidgetItem *buildChordItem(const QString);
};

#endif // CHORDTREE_H
