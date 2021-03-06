#ifndef TONALITY_H
#define TONALITY_H

#include <QString>
#include <QRegExp>
#include <QStringList>

/**
 * @brief Liste des fondamentales possibles.
 */
enum e_Note { A, B, C, D, E, F, G, VIDE };
/**
 * @brief Liste des altérations possibles.
 */
enum e_Alteration { UNALTERED, SHARP, FLAT };

/**
 * @brief Objet représentant une tonalité sous forme d'une note fondamentale et d'une altération.
 */
class Tonality
{
	public:
		Tonality();
		Tonality(const Tonality &t);
        Tonality(const QString note);
        Tonality(const e_Note note, const e_Alteration alteration = UNALTERED);

		bool isValid() const;
		bool isValid(const QString tonality) const;

        static e_Note extractBaseNoteFromStr(const QString str);
        static e_Alteration extractAlterationFromStr(const QString str);

		QString toString();
		e_Note getBaseNote() const;
		e_Alteration getAlteration() const;


        void setBaseNote(const QString note);
        void setBaseNote(const e_Note note);
        void setAlteration(const QString alteration);
        void setAlteration(const e_Alteration alteration);

        static QStringList getTonalitiesStringList();
private:
        QRegExp m_noteRegExp;
        e_Note m_baseNote;
        e_Alteration m_alteration;
};

#endif // TONALITY_H
