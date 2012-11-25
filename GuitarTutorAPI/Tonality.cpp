#include "Tonality.h"

Tonality::Tonality()
{
	m_baseNote = C;
	m_alteration = UNALTERED;
}

Tonality::Tonality(Tonality& t)
{
	m_baseNote = t.getBaseNote();
	m_alteration = t.getAlteration();
}

Tonality::Tonality(const QString& note)
{
	m_baseNote = extractBaseNoteFromStr(note);
	m_alteration = extractAlterationFromStr(note);
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}


Tonality::Tonality(const e_Note &note)
{
	m_baseNote = note;
	m_alteration = UNALTERED;
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

Tonality::Tonality(const e_Note& note, const e_Alteration& alteration)
{
	m_baseNote = note;
	m_alteration = alteration;
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

Tonality::Tonality(const QString& note, const QString& alteration)
{
	m_baseNote = extractBaseNoteFromStr(note);
	m_alteration = extractAlterationFromStr(alteration);
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}


bool Tonality::isValid()
{
	return (0 <= m_baseNote && m_baseNote <= 6 && 0 <= m_alteration && m_alteration <= 2);
}

bool Tonality::isValid(const QString& tonality)
{
	return tonality.size() <= 2 && tonality.contains(m_noteRegExp);
}

QString Tonality::toString()
{
	QString note = "ZZ";
	note[0]= m_baseNote + 65;

	if(m_alteration == SHARP)
	{
		note[1] = '#';
	}
	else if(m_alteration == FLAT)
	{
		note[1] = 'b';
	}
	else
	{
		note.resize(1);
	}

	return note;
}

e_Note Tonality::getBaseNote()
{
	return m_baseNote;
}

e_Alteration Tonality::getAlteration()
{
	return m_alteration;
}
void Tonality::setBaseNote(const QString& note)
{
	m_baseNote = extractBaseNoteFromStr(note);
}

void Tonality::setBaseNote(const e_Note& note)
{
	m_baseNote = note;
}

void Tonality::setAlteration(const QString& alteration)
{
	m_alteration = extractAlterationFromStr(alteration);
}

void Tonality::setAlteration(const e_Alteration& alteration)
{
	m_alteration = alteration;
}

e_Note Tonality::extractBaseNoteFromStr(const QString& str)
{
	return (e_Note) (str.at(0).toAscii() - 65);
}

e_Alteration Tonality::extractAlterationFromStr(const QString& str)
{
	if(str.size() > 1)
	{
		if(str.at(1) == 'b')
		{
			return FLAT;
		}
		else if(str.at(1) == '#')
		{
			return SHARP;
		}
	}

	return UNALTERED;
}
