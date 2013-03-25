#ifndef CASEITEMDELEGATE_H
#define CASEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "CaseItem.h"

/**
 * @brief Permet de dessiner ou d'ajouter de la couleur dans les case
 * de l'éditeur.
 */

class CaseItemDelegate : public QStyledItemDelegate
{
		Q_OBJECT
	public:
		explicit CaseItemDelegate(QWidget *parent = 0);
		~CaseItemDelegate();
		void paint(QPainter *painter, const QStyleOptionViewItem &option,
					   const QModelIndex &index) const;
		void paintPartText(QPainter *painter, const QRect& rect, CaseItem* item) const;
		void paintBar(QPainter *painter, const QRect& rect, CaseItem* item) const;
	signals:

	public slots:
		void setBarSize(int);


	private:
		int m_barsize;
		QFont * m_partFont;
		QPen * m_barPen;

};

#endif // CASEITEMDELEGATE_H
