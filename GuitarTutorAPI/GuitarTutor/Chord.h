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
<<<<<<< HEAD
		Chord(const QString& chord);
		Chord(const Chord &chord);
		Chord(const Tonality &tonality, const Enrichment &enrichment);
=======
        Chord(const QString chord);
		Chord(Chord& chord);
        Chord(Tonality& tonality, Enrichment& enrichment);
>>>>>>> 6aaf7988d503c4bdd6a0436129cc7c9b332bdfcd

		bool isValid();
		bool isValid(const QString& chord);

		QString toString();
		const Enrichment &getEnrichment() const;
		const Tonality& getTonality() const;

		Tonality& extractTonalityFromStr(const QString& str);
		Enrichment& extractEnrichmentFromStr(const QString& str);

		void setTonality(Tonality &tone);
		void setTonality(const QString& tone);
		void setEnrichment(Enrichment &enrichment);
		void setEnrichment(const QString &enrichment);


	private:
		Enrichment m_enrichment;
		Tonality m_tonality;
};

#endif // CHORD_H
