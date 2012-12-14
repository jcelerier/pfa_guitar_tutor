#include "Enrichment.h"
#include <QVector>
#include <QDebug>

/**
 * @brief Enrichment::Enrichment
 *
 * Constructeur par défaut.
 */
Enrichment::Enrichment()
{
    initStringEquivs();
}

/**
 * @brief Enrichment::Enrichment
 * @param enrich Enrichissement
 *
 * Constructeur par recopie.
 */
Enrichment::Enrichment(const Enrichment& enrich)  : QList<e_Enrichment>(enrich)
{
    initStringEquivs();
/* est-ce nécessaire ?
    while(!enrich.isEmpty())
    {
        append(enrich.takeFirst());
    }*/
}

/**
 * @brief Enrichment::Enrichment
 * @param enrich Enrichissement
 *
 * Crée un enrichissement à partir d'une chaîne de caractères.
 */
Enrichment::Enrichment(const QString enrich)
{
    initStringEquivs();
    Enrichment tmp;
    tmp = extractEnrichmentsFromStr(enrich);

    while(!tmp.isEmpty())
    {
        append(tmp.takeFirst());
    }
}

/**
 * @brief Enrichment::Enrichment
 * @param enrichList Enrichissement
 *
 * Crée un enrichissement à partir d'une liste de chaîne de caractères.
 */
Enrichment::Enrichment(const QStringList enrichList)
{
    QString e;
    Enrichment ebis;

    initStringEquivs();
    foreach(e, enrichList)
        for(int i = 0; i < (ebis = extractEnrichmentsFromStr(e)).size(); i++ ) //aurait été plus propre avec un deuxième foreach
            append(ebis[i]); //attention si la liste est mal formée
}

/**
 * @brief Enrichment::getEnrichmentStringList
 * @return L'enrichissement sous forme de liste de chaînes de caractères.
 */
QStringList& Enrichment::getEnrichmentStringList()
{
        return m_stringEquivs;
}

/**
 * @brief Enrichment::initStringEquivs
 *
 * Initialisation de la variable m_stringEquivs
 */
void Enrichment::initStringEquivs()
{
    m_stringEquivs << "M" <<"m"<<"+"<<"-"<<"sus2"<<"sus4"<<"b5" << "6" << "7" << "9" << "11" << "13";
}

/**
 * @brief Enrichment::isValid
 * @return Vrai si et seulement si l'enrichissement est valide.
 *
 * Indique si un enrichissement est valide.
 * @todo à approfondir. Ex: pas de C#Mmsus2sus4776b5... cf site sur le wiki
 */
bool Enrichment::isValid() const
{
	bool validity = false;
	for(int i = 0; i < size(); i++)
		validity = (0 <= at(i) && at(i) <= NUM_ENRICHMENTS);

	return validity;
}

/**
 * @brief Enrichment::isValid
 * @param enrichment Enrichissement
 * @return Vrai si est seulement si l'enrichissement est valide.
 *
 * Indique si un enrichissement provenant d'une chaîne de caractères est valide ou non.
 */
bool Enrichment::isValid(const QString& enrichment) const
{
	return Enrichment::extractEnrichmentsFromStr(enrichment).isValid();
}

/**
 * @brief Enrichment::extractEnrichmentsFromStr
 * @param str Chaîne de caractères à partir de laquelle faire l'extraction
 * @return L'enrichissement extrait.
 *
 * Extrait un enrichissement à partir d'une chaîne de caractères.
 */
// Algorithme:
// - on fait un tableau avec tous les enrichissements possibles.
// - on les recherche tous, puis on rajoute celui avec la plus petite valeur (= le plus proche du début).
// - on le cherche à nouveau (s'il y est deux fois, même si c'est invalide)
const Enrichment &Enrichment::extractEnrichmentsFromStr(QString str) const
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

/**
 * @brief Enrichment::toString
 * @return L'enrichissement en chaîne de caractères.
 *
 * Converti un enrichissement en chaîne de caractères.
 */
QString Enrichment::toString()
{
	QString str = "";
	for(int i = 0; i < size(); i++)
		str += m_stringEquivs[at(i)];

	return str;
}
