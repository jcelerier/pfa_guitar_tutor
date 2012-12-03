#ifndef CHORD_H
#define CHORD_H

#include "Enrichment.h"
#include "Tonality.h"
#include "GuitarTutorAPI_global.h"

// Fichier source de la classe Chord, représentant un accord.
class GUITARTUTORAPISHARED_EXPORT Chord
{
	public:
		Chord(const QString& chord);
		Chord(Chord& chord);
		Chord(Tonality& tonality, Enrichment& enrichment);
		Chord(Tonality &tonality);

		bool isValid();
		bool isValid(const QString& chord);

		QString toString();
		Enrichment getEnrichment();
		Tonality getTonality();

		Tonality extractTonalityFromStr(const QString& str);
		Enrichment extractEnrichmentFromStr(const QString& str);

		void setTonality(Tonality &tone);
		void setTonality(const QString& tone);
		void setEnrichment(Enrichment &enrichment);
		void setEnrichment(const QString &enrichment);


	private:
		Enrichment m_enrichment;
		Tonality m_tonality;
};

#endif // CHORD_H
