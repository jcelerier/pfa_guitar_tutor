#ifndef TONALITY_H
#define TONALITY_H

#include <QString>
#include <QRegExp>


enum Note { A, B, C, D, E, F, G };
enum Alteration { UNALTERED, SHARP, FLAT };

class Tonality
{
	public:
		Tonality();
		Tonality(QString& note, QString& alteration);
		Tonality(QString& note);
		Tonality(Note note);
		Tonality(QString& baseNote, int alteration);
		Tonality(Note note, Alteration alteration);

		bool isValidTonality();
		bool isValidTonality(QString& tonality);

		Note extractBaseNoteFromStr(QString& str);
		Alteration extractAlterationFromStr(QString& str);

		QString getNote();

		void setBaseNote(QString& note);
		void setBaseNote(Note note);
		void setAlteration(QString& alteration);
		void setAlteration(Alteration alteration);

	private:
		QRegExp m_noteRegExp;
		Note m_baseNote;
		Alteration m_alteration;
};

#endif // TONALITY_H
