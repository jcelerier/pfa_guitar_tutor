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
	QColor* m_color;
	QString m_part;
	QTime m_beginningTimer;
	bool m_timerManuallySet;
	bool m_partEditable;

	public:
		CaseItem(const bool partEditable = true);
		CaseItem(const QTableWidgetItem& item);
		~CaseItem();

		void set_color(int r, int g, int b, int);
		void set_chord(QString cw);
		QColor* get_color() const;
		QString get_chord() const;
		CaseItem* clone() const;
		void ShowContextMenu(const QPoint& pos);

		QString& getPart();
		void setPart(QString);
		void setBadChordColor();
		void setBadTimeColor();

		void paintEvent(QPaintEvent* event);

		bool isPartSet();
		void setBeginning(QTime t, bool timerManuallySet = false);
		QTime getBeginning();
		bool isTimerManuallySet();
		bool isPartEditable();

	public slots:
		void play();
		void restoreColor();
};

#endif // CASEITEM_H
