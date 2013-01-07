#ifndef CHORD_H
#define CHORD_H

#include <QtTest/QTest>
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
		Chord(const Chord &chord);
		Chord(const Tonality& tonality, const Enrichment& enrichment);

		bool isValid() const;
		bool isValid(const QString& chord) const;

		QString toString();
		const Enrichment &getEnrichment() const;
		const Tonality &getTonality() const;

        Tonality extractTonalityFromStr(const QString str) const;
        Enrichment extractEnrichmentFromStr(const QString str) const;

		void setTonality(Tonality &tone);
		void setTonality(const QString& tone);
		void setEnrichment(Enrichment &enrichment);
		void setEnrichment(const QString &enrichment);


	private:
		Enrichment m_enrichment;
		Tonality m_tonality;
};

#endif // CHORD_H
