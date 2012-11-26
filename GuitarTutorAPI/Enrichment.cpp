#include "Enrichment.h"
#include <QVector>
#include <QDebug>

/*
Enrichment::Enrichment()
{
	enrichmentStringList = (QStringList() << "" << "m" << "aug" << "dim" << "sus4" << "sus2");
	enrichmentQuantity = enrichmentStringList.size();
}*/

QStringList& Enrichment::getEnrichmentStringList()
{
        return m_stringEquivs;
}
/*
int Enrichment::getEnrichmentQuantity()
{
	return enrichmentQuantity;
}
*/

Enrichment::Enrichment()
{
	initStringEquivs();
}

Enrichment::Enrichment(Enrichment &enrich)  : QList<e_Enrichment>(enrich)
{
	initStringEquivs();
/* est-ce nécessaire ?
	while(!enrich.isEmpty())
	{
		append(enrich.takeFirst());
	}*/
}

Enrichment::Enrichment(const QString& enrich)
{
	initStringEquivs();
	Enrichment tmp;
	tmp = Enrichment::extractEnrichmentsFromStr(enrich);

	while(!tmp.isEmpty())
	{
		append(tmp.takeFirst());
	}
}

Enrichment::Enrichment(const QStringList& enrichList)
{
	QString e;
	Enrichment ebis;

	initStringEquivs();
	foreach(e, enrichList)
		for(int i = 0; i < (ebis = extractEnrichmentsFromStr(e)).size(); i++ ) //aurait été plus propre avec un deuxième foreach
			append(ebis[i]); //attention si la liste est mal formée
}

void Enrichment::initStringEquivs()
{
    m_stringEquivs << "M" <<"m"<<"+"<<"-"<<"sus2"<<"sus4"<<"b5" << "6" << "7" << "9" << "11" << "13";

}

// à approfondir. Ex: pas de C#Mmsus2sus4776b5... cf site sur le wiki
bool Enrichment::isValid()
{
	bool validity = false;
	for(int i = 0; i < size(); i++)
		validity = (0 <= at(i) && at(i) <= NUM_ENRICHMENTS);

	return validity;
}

bool Enrichment::isValid(const QString& enrichment)
{
	return Enrichment::extractEnrichmentsFromStr(enrichment).isValid();
}

// algorithme: on fait un tableau avec tous les enrichissements possibles.
// on les recherche tous, puis on rajoute celui avec la plus petite valeur (= le plus proche du début).
// on le cherche à nouveau (s'il y est deux fois, même si c'est invalide)
Enrichment Enrichment::extractEnrichmentsFromStr(QString str)
{
	Enrichment e;
	QVector<int> table(NUM_ENRICHMENTS, -1);
	int minIndex = 0;
	int minValue = 0;


	while(str.size() > 0)
	{
		for(int i = 0; i < NUM_ENRICHMENTS; i++)
		{
			table[i] = str.indexOf(m_stringEquivs[i]);
			if(table[i] > -1 && table[i] < minValue)
			{
				minValue = table[i];
				minIndex = i;
			}
		}

		e.append((e_Enrichment) minIndex);
		str.remove(m_stringEquivs[minIndex]);
	}


	return e;
}

QString Enrichment::toString()
{
	QString str = "";
	for(int i = 0; i < size(); i++)
		str += m_stringEquivs[at(i)];

	return str;
}
