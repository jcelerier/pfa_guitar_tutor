#include "Tonality.h"

Tonality::Tonality()
{
	m_baseNote = C;
	m_alteration = UNALTERED;
}

Tonality::Tonality(QString& note)
{
	m_baseNote = extractBaseNoteFromStr(note);
	m_alteration = extractAlterationFromStr(note);
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

Tonality::Tonality(QString& baseNote, int alteration)
{
	m_baseNote = extractBaseNoteFromStr(baseNote);
	m_alteration = (Alteration) alteration;
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

Tonality::Tonality(Note note)
{
	m_baseNote = note;
	m_alteration = UNALTERED;
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

Tonality::Tonality(Note note, Alteration alteration)
{
	m_baseNote = note;
	m_alteration = alteration;
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}

Tonality::Tonality(QString& note, QString& alteration)
{
	m_baseNote = extractBaseNoteFromStr(note);
	m_alteration = extractAlterationFromStr(alteration);
	m_noteRegExp = QRegExp("^([A-G]|[a-g]){1}(b#)?");
}


bool Tonality::isValidTonality()
{
	return (0 <= m_baseNote && m_baseNote <= 6 && 0 <= m_alteration && m_alteration <= 2);
}

bool Tonality::isValidTonality(QString& tonality)
{
	return tonality.size() <= 2 && tonality.contains(m_noteRegExp);
}

QString Tonality::getNote()
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

void Tonality::setBaseNote(QString& note)
{
	m_baseNote = extractBaseNoteFromStr(note);
}

void Tonality::setBaseNote(Note note)
{
	m_baseNote = note;
}

void Tonality::setAlteration(QString& alteration)
{
	m_alteration = extractAlterationFromStr(alteration);
}

void Tonality::setAlteration(Alteration alteration)
{
	m_alteration = alteration;
}

Note Tonality::extractBaseNoteFromStr(QString& str)
{
	return (Note) (str.at(0).toAscii() - 65);
}

Alteration Tonality::extractAlterationFromStr(QString& str)
{
	if(str.size() > 1)
	{
		if(str.at(2) == 'b')
		{
			return FLAT;
		}
		else if(str.at(2) == '#')
		{
			return SHARP;
		}
	}

	return UNALTERED;

}
