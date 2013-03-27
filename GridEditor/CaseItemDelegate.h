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
		void paint(QPainter *, const QStyleOptionViewItem &,
					   const QModelIndex &) const;
		void paintPartText(QPainter *, const QRect& , CaseItem* ) const;
		void paintBar(QPainter *, const QRect& , CaseItem* ) const;
	signals:

	public slots:
		void setBarSize(int);

	private:
		int m_barsize;
		QFont * m_partFont;
		QPen * m_barPen;

};

#endif // CASEITEMDELEGATE_H
