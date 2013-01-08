/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#ifndef CHORDTABLEWIDGET_H
#define CHORDTABLEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include "CaseItem.h"
#include "PartSetter.h"

/**
 * @brief Grille d'accords
 */
class ChordTableWidget : public QTableWidget
{
    Q_OBJECT

	QString* name;
	QMenu* m_rightClickMenu;
	CaseItem* m_currentItem;

    QAction* m_properties;

	PartSetter* m_setPartDialog;
protected:
	//virtual void mousePressEvent( QMouseEvent* event );

public:
    ChordTableWidget();
    ChordTableWidget(int column, int row);
    ChordTableWidget(QString xgrid_file);
    QString get_name() const;
    void set_name(QString name);
    void open_grid(QString xgrid_file);
    void to_xml(QString xml_file);
    bool is_selection_empty();
    bool is_row_selected();
    QList<QList<int>*> rows_selected();
    void insert_row(int position, int number = 1);

private:
    QList<int> expand_list(QList<QList<int>*> list);


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
	void removeCasePart();

	//void onClick(int row, int column);
};

#endif // CHORDTABLEWIDGET_H
