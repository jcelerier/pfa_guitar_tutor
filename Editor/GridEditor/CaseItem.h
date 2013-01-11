/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 14/04/12
*/

#ifndef CASEITEM_H
#define CASEITEM_H

#include <QColor>
#include <QSize>
#include <QTableWidgetItem>
#include <QTime>

/**
 * @brief Case d'une grille d'accords
 */
class CaseItem : public QTableWidgetItem
{
    QColor* m_color;
	QString m_part;
    QTime m_beginningTimer;
    bool m_timerManuallySet;

	public:
		CaseItem();
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

		bool isPartSet();
        void setBeginningTimer(QTime t, bool timerManuallySet = false);
        QTime getBeginning();
        bool isTimerManuallySet();
};

#endif // CASEITEM_H
