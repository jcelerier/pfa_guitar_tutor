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
	CaseItem* m_savedItem;
	CaseItem* m_currentItem;
	CaseItem* m_lastPlayedCase;
	CaseItemDelegate* m_caseItemDelegate;

	QAction* m_properties, *m_playFromHere;

	PartSetter* m_setPartDialog;
protected:
	//virtual void mousePressEvent( QMouseEvent* event );

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

private:
	QList<int> expand_list(QList<QList<int>*> );

signals:
	void play(int);
	void barsizeChanged(int);
	void somethingChanged();


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

	void setTimeInfo(const QTime , const QTime , const QTime );
	void isPlayingAt(QTime );

	void currentItemChanged_slot(QTableWidgetItem* , QTableWidgetItem*);
	void itemChanged_slot(QTableWidgetItem* );

	void setBarSize(int);
};

#endif // CHORDTABLEWIDGET_H
