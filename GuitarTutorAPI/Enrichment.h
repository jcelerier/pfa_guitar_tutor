#ifndef TUNING_H
#define TUNING_H

#include <QList>
#include <QStringList>
#include "GuitarTutorAPI_global.h"

// Un accord peut avoir plusieurs enrichissements. Exemple : C#mM7 = C#, mineur, 7ième majeure

enum e_Enrichment { MAJ, MIN, AUG, DIM, SUS2, SUS4, B5,  SIX, SEPT, NEUF, ONZE, TREIZE};
#define NUM_ENRICHMENTS 12
class GUITARTUTORAPISHARED_EXPORT Enrichment : public QList<e_Enrichment>
{
	public:
		Enrichment();
		Enrichment(Enrichment &enrich);
		Enrichment(const QString& enrich);
		Enrichment(const QStringList& enrichList);

		void initStringEquivs();
		bool isValid();
		bool isValid(const QString& enrichment);

		Enrichment extractEnrichmentsFromStr(QString str);

		QString toString();
                QStringList& getEnrichmentStringList();

	private:
		QStringList m_stringEquivs;
};

#endif // TUNING_H
