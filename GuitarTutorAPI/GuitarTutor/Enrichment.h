#ifndef TUNING_H
#define TUNING_H

#include <QList>
#include <QStringList>
#include <QVector>
#include <QtTest/QTest>
#include <QDebug>
#include <iostream>

// Un accord peut avoir plusieurs enrichissements. Exemple : C#mM7 = C#, mineur, 7ième majeure

#define NUM_ENRICHMENTS 12

/**
 * @brief Enumération des composantes possibles d'un enrichissement
 */
enum e_Enrichment { MAJ, MIN, AUG, DIM, SUS2, SUS4, B5, SIX, SEPT, NEUF, ONZE, TREIZE};
// Ces accord sont stcké sous forme de QString :
//"M"= MAJ, "m"= MIN, "+"= AUG, "-"=DIM, "sus2"=SUS2, "sus4=SUS4", "b5"=B5, "6"=SIX, "7"=SEPT
//"9"=NEUF, "11"=ONZE, "13"=TREIZE
//Quelle est l'utilité de l'énum?

/**
 * @brief Objet enrichissement pour définir la forme d'un accord
 */
class Enrichment : public QList<e_Enrichment>
{
	public:
        static QStringList listOfEnrichmentString;
		static const QStringList& getListOfEnrichmentStrings();

		Enrichment();
        Enrichment(const Enrichment &enrich);
        Enrichment(const QString &enrich);
        Enrichment(const QStringList enrichList);
        Enrichment(const e_Enrichment enrich);

		bool isValid() const;
        bool isValid(const QString& enrichment) const;
        bool isEmpty() const;

		void addEnrichment(const QString &newEnrich);
        const Enrichment extractEnrichmentsFromStr(const QString &str_enr) const;

		QString toString();

		const QStringList& getEnrichmentStringList() const;
		const QStringList& getListOfEnrichmentStringList() const;

	private:
        void initStringEquivs();
        QStringList m_stringEquivs;
};

#endif // TUNING_H
