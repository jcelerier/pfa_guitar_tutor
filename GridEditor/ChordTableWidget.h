#ifndef CHORDTABLEWIDGET_H
#define CHORDTABLEWIDGET_H

#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QList>
#include <QPoint>

#include "CaseItem.h"
#include "CaseItemDelegate.h"

#include "PartSetter.h"
#include "Track/LogicalTrack.h"

class GridEditor;

/**
 * @brief Grille d'accords
 */
class ChordTableWidget : public QTableWidget
{
		Q_OBJECT

		friend class PartSetter;
		friend class CaseItemDelegate;

	public:
		ChordTableWidget(int , int , QWidget *parent);
		~ChordTableWidget();

		bool isSelectionEmpty();
		bool isRowSelected();
		QList<QList<int>*> rowsSelected();
		void insertChordRow(int , int number = 1);

		LogicalTrack* getLogicalTrack();
		void setLogicalTrack(LogicalTrack* );

		void deepCopy(ChordTableWidget* );

	public slots:
		void ShowContextMenu(const QPoint& );
		void showProperties();

		void fill_selection(QTreeWidgetItem* , int );
		void insertChordRow();
		void insertChordColumn();
		void deleteSelectedColumn();
		void deleteSelectedRow();
		void copyDownRows();

		void setCasePart(QString );
		void setCaseBeginning(QTime );
		void setCaseAndFollowersBeginning(QTime );
		void removeCasePart();
		bool checkBeginningTimes();

		void playFromHere();

		void setTimeInfo(const QTime, const QTime, const QTime);
		void isPlayingAt(QTime);

		void currentItemChanged_slot(QTableWidgetItem* , QTableWidgetItem*);
		void itemChanged_slot(QTableWidgetItem* );

		void setBarSize(int);

	signals:
		/**
		 * @brief play est envoyé quand la lecture commence lorsqu'on veut la faire partir à partir d'une case précise
		 */
		void play(int);

		/**
		 * @brief barsizeChanged est envoyé quand la taille de mesure change, pour répercuter sur CaseItemDelegate
		 */
		void barsizeChanged(int);

		/**
		 * @brief somethingChanged est envoyé quand une donnée de l'application change : sert à la sauvegarde
		 */
		void somethingChanged();

	private:
		QList<int> expand_list(QList<QList<int>*> );

		/**
		 * @brief m_barsize la taille de mesure
		 */
		int m_barsize;

		/**
		 * @brief m_rightClickMenu le menu de clic droit sur une case
		 */
		QMenu* m_rightClickMenu;

		/**
		 * @brief m_savedItem est utilisé pour vérifier si on change des données quand on change d'objet (sert à la sauvegarde)
		 */
		CaseItem* m_savedItem;

		/**
		 * @brief m_currentItem pointeur sur l'item actuellement sélectionné (en bleu foncé généralement)
		 */
		CaseItem* m_currentItem;

		/**
		 * @brief m_lastPlayedCase pointeur sur la dernière case à avoir été lue
		 */
		CaseItem* m_lastPlayedCase;

		/**
		 * @brief m_caseItemDelegate sert à changer le style graphique des CaseItem
		 */
		CaseItemDelegate* m_caseItemDelegate;

		/**
		 * @brief m_properties Action déclenchée pour ouvrir le menu de propriétés
		 */
		QAction* m_properties;

		/**
		 * @brief m_playFromHere Action pour lancer la lecture à partir d'une case
		 */
		QAction* m_playFromHere;

		/**
		 * @brief m_setPartDialog Dialogue qui s'ouvre pour modifier les propriétés d'une case
		 */
		PartSetter* m_setPartDialog;

};

#endif // CHORDTABLEWIDGET_H
