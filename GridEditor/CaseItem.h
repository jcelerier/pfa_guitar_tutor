#ifndef CASEITEM_H
#define CASEITEM_H

#include <QtWidgets/QTableWidgetItem>
#include <QTime>
#include <QPoint>

#include "Chord.h"
/**
 * @brief Case d'une grille d'accords
 */
class CaseItem : public QTableWidgetItem
{
	public:
		CaseItem(const bool partEditable = true);
		CaseItem(const CaseItem &item);
		~CaseItem();

		void setColor(int r, int g, int b, int a = 255);
		void setChord(QString );
		QString getChord() const;
		CaseItem* clone() const;

		QString& getPart();
		void setPart(QString);
		void setBadChordColor();
		void setBadTimeColor();
		void setPlayColor();
		bool isBeingPlayed();

		bool isPartSet();
		void setBeginning(QTime, bool timerManuallySet = false);
		QTime getBeginning();
		bool isTimerManuallySet();
		bool isPartEditable();

		void play(bool value = true);
		void restoreColor();

	private:
		/**
		 * @brief m_brush Brosse qui sert à dessiner la couleur d'arrière-plan
		 */
		QBrush* m_brush;

		/**
		 * @brief m_part La partie de la case
		 */
		QString m_part;

		/**
		 * @brief m_beginningTimer L'instant ou la case commence
		 */
		QTime m_beginningTimer;

		/**
		 * @brief m_timerManuallySet Booléen qui dit si le timer a été changé à la main (pour demander confirmation d'écrasement)
		 */
		bool m_timerManuallySet;

		/**
		 * @brief m_partEditable Booléen vrai si on peut changer la partie
		 */
		bool m_partEditable;

		/**
		 * @brief m_currentlyPlaying Booléen vrai si la partie est en train d'être lue
		 */
		bool m_currentlyPlaying;
};

#endif // CASEITEM_H
