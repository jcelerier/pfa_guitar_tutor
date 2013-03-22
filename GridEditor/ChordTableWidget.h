/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#ifndef CHORDTABLEWIDGET_H
#define CHORDTABLEWIDGET_H

#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTableWidget>
#include <QList>
#include <QtWidgets/QMenu>
#include <QString>
#include <QtWidgets/QAction>
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

	int m_barsize;

	QMenu* m_rightClickMenu;
	CaseItem* m_currentItem;
	CaseItem* m_lastPlayedCase;
	CaseItemDelegate* m_caseItemDelegate;

	QAction* m_properties, *m_playFromHere;

	PartSetter* m_setPartDialog;
protected:
	//virtual void mousePressEvent( QMouseEvent* event );

public:
	ChordTableWidget(int column, int row, QWidget *parent);
	~ChordTableWidget();

	bool is_selection_empty();
	bool is_row_selected();
	QList<QList<int>*> rows_selected();
	void insert_row(int position, int number = 1);

	LogicalTrack* getLogicalTrack();
	void setLogicalTrack(LogicalTrack* track);

	void deepCopy(ChordTableWidget* target);

private:
	QList<int> expand_list(QList<QList<int>*> list);

signals:
	void play(int);
	void barsizeChanged(int);

public slots:
	void ShowContextMenu(const QPoint& pos);
	void showProperties();

	void fill_selection(QTreeWidgetItem* chord, int column);
	void insert_row();
	void insert_column();
	void delete_selected_column();
	void delete_selected_row();
	void copy_down_rows();

	void setCasePart(QString text);
	void setCaseBeginning(QTime t);
	void setCaseAndFollowersBeginning(QTime t);
	void removeCasePart();
	bool checkBeginningTimes();

	void playFromHere();

    void setTimeInfo(const QTime beginning, const QTime bar, const QTime end);
	void isPlayingAt(QTime t);

	void itemChanged_slot(QTableWidgetItem* item);

	void setBarSize(int);

	//void onClick(int row, int column);
};

#endif // CHORDTABLEWIDGET_H
