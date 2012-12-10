#include "Chord.h"

/**
 * @brief Chord::Chord
 * @param chord Chaîne de caractères à partir de laquelle former l'accord
 *
 * Construitun accord à partir d'une chaîne de caractères.
 */
Chord::Chord(const QString chord)
{
	m_tonality = extractTonalityFromStr(chord);
	m_enrichment = extractEnrichmentFromStr(chord);
}

<<<<<<< HEAD
Chord::Chord(const Chord& chord)
=======
/**
 * @brief Chord::Chord
 * @param chord Accord
 *
 * Constructeur par recopie.
 */
Chord::Chord(Chord& chord)
>>>>>>> 6aaf7988d503c4bdd6a0436129cc7c9b332bdfcd
{
	m_tonality = chord.getTonality();
	m_enrichment = chord.getEnrichment();
}

<<<<<<< HEAD
Chord::Chord(const Tonality& tonality, const Enrichment& enrichment)
=======
/**
 * @brief Chord::Chord
 * @param tonality Tonalité
 * @param enrichment Enrichissement
 *
 * Construit un accord à partir d'une tonalité et d'un enrichissement.
 */
Chord::Chord(Tonality &tonality, Enrichment &enrichment)
>>>>>>> 6aaf7988d503c4bdd6a0436129cc7c9b332bdfcd
{
	m_tonality = Tonality(tonality);
	m_enrichment = Enrichment(enrichment);
}

/**
 * @brief Chord::isValid
 * @return Vrai si est seulement si l'accord est valide.
 *
 * Indique si un accord est valide ou non.
 */
bool Chord::isValid()
{
	return m_tonality.isValid() && m_enrichment.isValid();
}

/**
 * @brief Chord::isValid
 * @param chord Accord
 * @return Vrai si est seulement si l'accord extrait depuis la chaîne de caractères est valide.
 *
 * Indique si un accord est valide ou non.
 */
bool Chord::isValid(const QString &chord)
{
	return extractTonalityFromStr(chord).isValid() && extractEnrichmentFromStr(chord).isValid();
}

/**
 * @brief Chord::toString
 * @return L'accord sous forme de chaîne de caractères.
 *
 * Convertie un accord en chaîne de caractères.
 */
QString Chord::toString()
{
	QString chord = "";
	chord += m_tonality.toString();
	chord += m_enrichment.toString();

	return chord;
}

<<<<<<< HEAD
const Enrichment& Chord::getEnrichment() const
=======
/**
 * @brief Chord::getEnrichment
 * @return L'enrichissement de l'accord.
 *
 * Extrait l'enrichissement de l'accord.
 */
Enrichment Chord::getEnrichment()
>>>>>>> 6aaf7988d503c4bdd6a0436129cc7c9b332bdfcd
{
	return m_enrichment;
}

<<<<<<< HEAD
const Tonality& Chord::getTonality() const
=======
/**
 * @brief Chord::getTonality
 * @return La tonalité de l'accord.
 *
 * Extrait la tonalité de l'accord.
 */
Tonality Chord::getTonality()
>>>>>>> 6aaf7988d503c4bdd6a0436129cc7c9b332bdfcd
{
	return m_tonality;
}

/**
 * @brief Chord::setTonality
 * @param tone Tonalité
 *
 * Modifie la tonalité de l'accord.
 */
void Chord::setTonality(Tonality& tone)
{
	m_tonality = Tonality(tone);
}

/**
 * @brief Chord::setTonality
 * @param tone Tonalité
 *
 * Modifie la tonalité de l'accord à partir d'une chaîne de caractères.
 */
void Chord::setTonality(const QString& tone)
{
	m_tonality = Tonality(tone);
}

/**
 * @brief Chord::setEnrichment
 * @param enrichment Enrichissement
 *
 * Modifie l'enrichissement de l'accord.
 */
void Chord::setEnrichment(Enrichment& enrichment)
{
    m_enrichment = Enrichment(enrichment);
}

/**
 * @brief Chord::setEnrichment
 * @param enrichment Enrichissement
 *
 * Modifie l'enrichissement d'un accord à partir d'une chaîne de caractères.
 */
void Chord::setEnrichment(const QString& enrichment)
{
	m_enrichment = Enrichment(enrichment);
}

<<<<<<< HEAD
Tonality& Chord::extractTonalityFromStr(const QString& str)
=======
/**
 * @brief Chord::extractTonalityFromStr
 * @param str Tonalité ou accord
 * @return La tonalité extraite.
 *
 * Extrait la tonalité d'une tonalité ou d'un accord sous forme de chaîne de caractères.
 */
Tonality Chord::extractTonalityFromStr(const QString& str)
>>>>>>> 6aaf7988d503c4bdd6a0436129cc7c9b332bdfcd
{
	Tonality tone;
    //On prend la sous-chaine de 2 ou 1 caractères correspondant à une tonalité valide
    if(str[1] == 'b' || str[1] == '#')
		tone = Tonality(str.left(2));
    else
		tone = Tonality(str.left(1));

	return tone;
}

<<<<<<< HEAD
Enrichment& Chord::extractEnrichmentFromStr(const QString& str)
=======
/**
 * @brief Chord::extractEnrichmentFromStr
 * @param str Accord
 * @return L'enrichissement extrait.
 *
 * Extrait l'enrichissement d'un accord sous forme de chaîne de caractères.
 */
Enrichment Chord::extractEnrichmentFromStr(const QString& str)
>>>>>>> 6aaf7988d503c4bdd6a0436129cc7c9b332bdfcd
{
	Enrichment enrich;
	if(str.at(1) == 'b' || str.at(1) == '#')
		enrich = Enrichment(str.right(2));
	else
		enrich = Enrichment(str.right(1));

	return enrich;
}
