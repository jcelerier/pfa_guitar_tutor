#include "Chord.h"

Chord::Chord(const QString& chord)
{
	m_tonality = extractTonalityFromStr(chord);
	m_enrichment = extractEnrichmentFromStr(chord);
}

Chord::Chord(Chord& chord)
{
	m_tonality = chord.getTonality();
	m_enrichment = chord.getEnrichment();
}

Chord::Chord(Tonality &tonality, Enrichment &enrichment)
{
	m_tonality = Tonality(tonality);
	m_enrichment = Enrichment(enrichment);
}

bool Chord::isValid()
{
	return m_tonality.isValid() && m_enrichment.isValid();
}

bool Chord::isValid(const QString &chord)
{
	return extractTonalityFromStr(chord).isValid() && extractEnrichmentFromStr(chord).isValid();
}

QString Chord::toString()
{
	QString chord = "";
	chord += m_tonality.toString();
	chord += m_enrichment.toString();

	return chord;
}

Enrichment Chord::getEnrichment()
{
	return m_enrichment;
}

Tonality Chord::getTonality()
{
	return m_tonality;
}

void Chord::setTonality(Tonality& tone)
{
	m_tonality = Tonality(tone);
}

void Chord::setTonality(const QString& tone)
{
	m_tonality = Tonality(tone);
}

void Chord::setEnrichment(Enrichment& enrichment)
{
    m_enrichment = Enrichment(enrichment);
}

void Chord::setEnrichment(const QString& enrichment)
{
	m_enrichment = Enrichment(enrichment);
}

Tonality Chord::extractTonalityFromStr(const QString& str)
{
	Tonality tone;
    //On prend la sous-chaine de 2 ou 1 caractères correspondant à une tonalité valide
    if(str[1] == 'b' || str[1] == '#')
		tone = Tonality(str.left(2));
    else
		tone = Tonality(str.left(1));

	return tone;
}

Enrichment Chord::extractEnrichmentFromStr(const QString& str)
{
	Enrichment enrich;
	if(str.at(1) == 'b' || str.at(1) == '#')
		enrich = Enrichment(str.right(2));
	else
		enrich = Enrichment(str.right(1));

	return enrich;
}
