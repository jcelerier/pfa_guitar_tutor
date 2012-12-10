#ifndef CHORD_H
#define CHORD_H

#include "Enrichment.h"
#include "Tonality.h"
#include "GuitarTutorAPI_global.h"

/**
 * @brief Représente un accord sous forme d'une tonalité et d'un enrichissement
 */
class GUITARTUTORAPISHARED_EXPORT Chord
{
	public:
        Chord(const QString chord);
		Chord(Chord& chord);
        Chord(Tonality& tonality, Enrichment& enrichment);

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
