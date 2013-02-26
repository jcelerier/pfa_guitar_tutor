#include "Tonality.h"

/**
 * @brief Tonality::Tonality
 *
 * Constructeur par défaut (Do).
 */
Tonality::Tonality()
{
	m_baseNote = VIDE;
    m_alteration = UNALTERED;
}

/**
 * @brief Tonality::Tonality
 * @param t Tonalité
 *
 * Constructeur par recopie.
 */
Tonality::Tonality(const Tonality& t)
{
    m_baseNote = t.getBaseNote();
    m_alteration = t.getAlteration();
}

/**
 * @brief Tonality::Tonality
 * @param note Note
 * @param alteration Altération
 *
 * Construit une tonalité à partir d'une note et d'une altération.
 */
Tonality::Tonality(const e_Note note, const e_Alteration alteration /*=UNALTERED*/)
{
    m_baseNote = note;
    m_alteration = alteration;
    m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

/**
 * @brief Tonality::Tonality
 * @param note Note
 * @param alteration Altération
 *
 * Construit une tonalité à partir d'une note et d'une altération sous forme de chaînes de caractères.
 */
Tonality::Tonality(const QString note)
{
    m_baseNote = extractBaseNoteFromStr(note);
    m_alteration = extractAlterationFromStr(note);
    m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

/**
 * @brief Tonality::isValid
 * @return Vrai si et seulement si la tonalité est valide.
 *
 * Indique si une tonalité est valide ou non.
 */
bool Tonality::isValid() const
{
    return (0 <= m_baseNote && m_baseNote <= 6 && 0 <= m_alteration && m_alteration <= 2);
}

/**
 * @brief Tonality::isValid
 * @param tonality Tonalité
 * @return Vrai si et seulement si la tonalité est valide.
 *
 * Indique si une tonalité est valide ou non.
 */
bool Tonality::isValid(const QString tonality) const
{
    return tonality.size() <= 2 && tonality.contains(m_noteRegExp);
}

/**
 * @brief Tonality::toString
 * @return La tonalité en chaîne de caractères.
 *
 * Converti une tonalité en chaîne de caractères.
 */
QString Tonality::toString()
{
	QString note = "";
	if(m_baseNote != VIDE)
	{
		QChar base = m_baseNote + 65;
		note.append(base);

		if(m_alteration == SHARP){
			note.append('#');
		}
		else if(m_alteration == FLAT){
			note.append('b');
		}
	}
	else
	{
		note = "nc";
	}
    return note;
}

/**
 * @brief Tonality::getBaseNote
 * @return La fondamentale.
 *
 * Renvoit la note fondamentale de la tonalité.
 */
e_Note Tonality::getBaseNote() const
{
    return m_baseNote;
}

/**
 * @brief Tonality::getAlteration
 * @return L'altération.
 *
 * Renvoit l'altération de la tonalité.
 */
e_Alteration Tonality::getAlteration() const
{
    return m_alteration;
}

/**
 * @brief Tonality::setBaseNote
 * @param note Note
 *
 * Modifie la fondamentale de la tonalité.
 */
void Tonality::setBaseNote(const QString note)
{
    m_baseNote = extractBaseNoteFromStr(note);
}

/**
 * @brief Tonality::setBaseNote
 * @param note Note
 *
 * Modifie la fondamentale de la tonalité.
 */
void Tonality::setBaseNote(const e_Note note)
{
    m_baseNote = note;
}

/**
 * @brief Tonality::setAlteration
 * @param alteration Altération
 *
 * Modifie l'altération de la tonalité.
 */
void Tonality::setAlteration(const QString alteration)
{
    m_alteration = extractAlterationFromStr(alteration);
}

/**
 * @brief Tonality::setAlteration
 * @param alteration Altération
 *
 * Modifie l'altération de la tonalité.
 */
void Tonality::setAlteration(const e_Alteration alteration)
{
    m_alteration = alteration;
}

/**
 * @brief Tonality::extractBaseNoteFromStr
 * @param str Chaîne de caractères
 * @return La note extraite
 *
 * Renvoit la note extraite d'une chaîne de caractères.
 */
e_Note Tonality::extractBaseNoteFromStr(const QString str)
{
    e_Note a = (e_Note) (str.at(0).toLatin1() - 65);
    return a;
}

/**
 * @brief Tonality::extractAlterationFromStr
 * @param str Chaîne de caractères
 * @return L'altération extraite
 *
 * Renvoit l'altération extraite d'une chaîne de caractères.
 */
e_Alteration Tonality::extractAlterationFromStr(const QString str)
{
    if(str.size() > 1)
    {
        if(str.at(1) == 'b')
            return FLAT;
        else if(str.at(1) == '#')
            return SHARP;
    }

    return UNALTERED;
}
