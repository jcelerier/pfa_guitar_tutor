#ifndef CHORDD_H
#define CHORDD_H

#include <QList>
#include <QStringList>
#include <QtTest/QTest>
#include "Enrichment.h"
#include "Tonality.h"

/**
 * @brief Représente un accord sous forme d'une tonalité et d'un enrichissement
 */
class Chord
{
	public:
        Chord(const QString &chord);
		Chord(const Chord &chord);
		Chord(const Tonality& tonality, const Enrichment& enrichment);

		bool isValid() const;
		bool isValid(const QString& chord) const;

		const Enrichment &getEnrichment() const;
		const Tonality &getTonality() const;

        Tonality extractTonalityFromStr(const QString str) const;
        const Enrichment extractEnrichmentFromStr(QString const str_enr) const;


		void setTonality(Tonality &tone);
		void setTonality(const QString& tone);
		void setEnrichment(Enrichment &enrichment);
		void setEnrichment(const QString &enrichment);

        QString toString();

	private:
		Enrichment m_enrichment;
		Tonality m_tonality;
};

#endif // CHORD_H
