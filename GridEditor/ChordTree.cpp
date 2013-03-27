#include <ChordTree.h>


/**
 * @brief ChordTree::ChordTree
 *
 * Constructeur de l'arbre de sélection d'accords.
 */
ChordTree::ChordTree() :
	QTreeWidget()
{
	setFixedWidth(200);
	setHeaderLabel(tr("Chord choice"));
	setDisabled(true); //Désactivé par défaut tant que new_button n'a pas été déclenché

	/*Ajout des tonalités*/
    QStringList tonalities = Tonality::getTonalitiesStringList();
    for(int i=0; i<tonalities.size(); i++)
		addTopLevelItem(buildChordItem(tonalities[i]));
}

/**
 * @brief ChordTree::buildChord
 * @param base_name Tonalité à rajouter à la liste des accords
 * @return L'élément à rajouter à la liste des accords.
 *
 * Cette fonction crée un objet contenant les différentes possibilités d'accords à partir d'une tonalité donnée: mineur, majeur,...
 */
QTreeWidgetItem* ChordTree::buildChordItem(const QString tonality) {

	Enrichment* tmp = new Enrichment();
	QStringList tuning = tmp->getListOfEnrichmentStringList();

	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText(0, tonality);

	QTreeWidgetItem* children[tuning.size()];

	for(int i=0; i<tuning.size(); i++)
	{
		children[i] = new QTreeWidgetItem();
		children[i]->setText(0, tonality + tuning[i]);
		item->addChild(children[i]);
	}
	delete tmp;
	return item;
}

