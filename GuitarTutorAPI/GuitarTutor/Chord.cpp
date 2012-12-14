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

/**
 * @brief Chord::Chord
 * @param chord Accord
 *
 * Constructeur par recopie.
 */
Chord::Chord(const Chord& chord)
{
	m_tonality = chord.getTonality();
	m_enrichment = chord.getEnrichment();
}

/**
 * @brief Chord::Chord
 * @param tonality Tonalité
 * @param enrichment Enrichissement
 *
 * Construit un accord à partir d'une tonalité et d'un enrichissement.
 */
Chord::Chord(const Tonality &tonality, const Enrichment &enrichment)
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
bool Chord::isValid() const
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
bool Chord::isValid(const QString &chord) const
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

/**
 * @brief Chord::getEnrichment
 * @return L'enrichissement de l'accord.
 *
 * Extrait l'enrichissement de l'accord.
 */
const Enrichment& Chord::getEnrichment() const
{
	return m_enrichment;
}

/**
 * @brief Chord::getTonality
 * @return La tonalité de l'accord.
 *
 * Extrait la tonalité de l'accord.
 */
const Tonality& Chord::getTonality() const
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

/**
 * @brief Chord::extractTonalityFromStr
 * @param str Tonalité ou accord
 * @return La tonalité extraite.
 *
 * Extrait la tonalité d'une tonalité ou d'un accord sous forme de chaîne de caractères.
 */
const Tonality Chord::extractTonalityFromStr(const QString& str) const
{
	Tonality tone;
    //On prend la sous-chaine de 2 ou 1 caractères correspondant à une tonalité valide
    if(str[1] == 'b' || str[1] == '#')
		tone = Tonality(str.left(2));
    else
		tone = Tonality(str.left(1));

	return tone;
}

/**
 * @brief Chord::extractEnrichmentFromStr
 * @param str Accord
 * @return L'enrichissement extrait.
 *
 * Extrait l'enrichissement d'un accord sous forme de chaîne de caractères.
 */
const Enrichment Chord::extractEnrichmentFromStr(const QString& str) const
{
	Enrichment enrich;
	if(str.at(1) == 'b' || str.at(1) == '#')
		enrich = Enrichment(str.right(2));
	else
		enrich = Enrichment(str.right(1));

	return enrich;
}
