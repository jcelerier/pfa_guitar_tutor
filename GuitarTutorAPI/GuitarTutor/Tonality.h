#ifndef TONALITY_H
#define TONALITY_H

#include <QString>
#include <QRegExp>
#include "GuitarTutorAPI_global.h"

/**
 * @brief Liste des fondamentales possibles.
 */
enum e_Note { A, B, C, D, E, F, G }; //possibilité d'en faire des classes? pas très utile amha
/**
 * @brief Liste des altérations possibles.
 */
enum e_Alteration { UNALTERED, SHARP, FLAT };

/**
 * @brief Objet représentant une tonalité sous forme d'une note fondamentale et d'une altération.
 */
class GUITARTUTORAPISHARED_EXPORT Tonality
{
	public:
		Tonality();
		Tonality(Tonality &t);
        Tonality(const QString& note, const QString& alteration = "UNALTERED");
        Tonality(const e_Note& note, const e_Alteration& alteration = UNALTERED);

		bool isValid();
		bool isValid(const QString& tonality);

		static e_Note extractBaseNoteFromStr(const QString& str);
		static e_Alteration extractAlterationFromStr(const QString& str);

		QString toString();
		e_Note getBaseNote();
		e_Alteration getAlteration();

		void setBaseNote(const QString& note);
		void setBaseNote(const e_Note& note);
		void setAlteration(const QString& alteration);
		void setAlteration(const e_Alteration& alteration);

	private:
        QRegExp m_noteRegExp;
		e_Note m_baseNote;
		e_Alteration m_alteration;
};

#endif // TONALITY_H
