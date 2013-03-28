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

	public slots:
		void setBarSize(int);

	private:
		/**
		 * @brief m_barsize La taille de la mesure
		 */
		int m_barsize;

		/**
		 * @brief m_partFont La police utilisée pour afficher le texte de partie
		 */
		QFont * m_partFont;

		/**
		 * @brief m_barPen Le pinceau utilisé pour dessiner les barres de mesure
		 */
		QPen * m_barPen;

};

#endif // CASEITEMDELEGATE_H
