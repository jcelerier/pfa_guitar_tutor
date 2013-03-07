#ifndef CHORDD_H
#define CHORDD_H

#include <QList>
#include <QStringList>
#include "Enrichment.h"
#include "Tonality.h"

/**
 * @brief Représente un accord sous forme d'une tonalité et d'un enrichissement
 */
class BasicChord
{
	public:
        BasicChord(const QString &chord);
        BasicChord(const BasicChord &chord);
        BasicChord(const Tonality& tonality, const Enrichment& enrichment);

        ~BasicChord();

		bool isValid() const;
        static bool isValid(const QString& chord);
        static bool isValidForPlayer(const QString& chord);

		const Enrichment &getEnrichment() const;
		const Tonality &getTonality() const;

        static Tonality extractTonalityFromStr(const QString str);
        static const Enrichment &extractEnrichmentFromStr(QString const str_enr);


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
