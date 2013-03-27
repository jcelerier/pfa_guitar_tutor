/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 14/04/12
*/

#ifndef CASEITEM_H
#define CASEITEM_H

#include <QColor>
#include <QSize>
#include <QtWidgets/QTableWidgetItem>
#include <QTime>
#include <QString>
#include <QPoint>
#include "Chord.h"
/**
 * @brief Case d'une grille d'accords
 */
class CaseItem : public QTableWidgetItem
{
	QBrush* m_brush;
	QString m_part;
	QTime m_beginningTimer;
	bool m_timerManuallySet;
	bool m_partEditable;
	bool m_currentlyPlaying;

	public:
		CaseItem(const bool partEditable = true);
		CaseItem(const CaseItem &item);
		~CaseItem();

		void setColor(int r, int g, int b, int a = 255);
		void setChord(QString cw);
		QString getChord() const;
		CaseItem* clone() const;

		QString& getPart();
		void setPart(QString);
		void setBadChordColor();
		void setBadTimeColor();
		void setPlayColor();
		bool isBeingPlayed();

		bool isPartSet();
		void setBeginning(QTime t, bool timerManuallySet = false);
		QTime getBeginning();
		bool isTimerManuallySet();
		bool isPartEditable();

	public slots:
		void play(bool value = true);
		void restoreColor();
};

#endif // CASEITEM_H
