/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 14/04/12
*/

#include <iostream>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QTextStream>
#include "CaseItem.h"
#include "ChordTableWidget.h"

ChordTableWidget::ChordTableWidget() : QTableWidget(10, 5)
{
    this->setHorizontalHeaderItem(4, new QTableWidgetItem("Annotation"));
    for (int c = 0 ; c < this->columnCount() - 1 ; c ++)
        for (int r = 0 ; r < this->rowCount() ; r ++)
        {
            this->setItem(r, c, new CaseItem());
            this->setColumnWidth(c, 60);
        }
    for (int r = 0 ; r < this->rowCount() ; r ++)
    {
        this->setItem(r, this->columnCount() - 1, new QTableWidgetItem());
        this->setRowHeight(r, 40);
    }
}

ChordTableWidget::ChordTableWidget(QString xgrid_file)
{
    ChordTableWidget();
    open_grid(xgrid_file);
}

void ChordTableWidget::open_grid(QString xgrid_file)
{
    QDomDocument doc("XGRID");
    QFile xml_doc(xgrid_file);
    if (!xml_doc.open(QIODevice::ReadOnly))
        exit(1);
    doc.setContent(&xml_doc);
    xml_doc.close();
    QDomElement chord_grid = doc.documentElement();
    insert_row(this->rowCount(), chord_grid.attribute("lines_number").toInt() - this->rowCount());
    QDomNode line_node = chord_grid.firstChild();
    for (int row = 0 ; !line_node.isNull() ; line_node = line_node.nextSibling(), row ++)
    {
        QDomElement line = line_node.toElement();
        QDomNode case_node = line.firstChild();
        for (int column = 0 ; !case_node.nextSibling().isNull() ; case_node = case_node.nextSibling(), column ++)
        {
            QDomElement case_e = case_node.toElement();
            CaseItem* item = (CaseItem*) this->takeItem(row, column);
            item->set_chord(case_e.firstChild().toElement().attribute("name"));
            int r = -1, g = -1, b = -1, a = -1;
            r = case_e.lastChild().toElement().attribute("R").toInt();
            g = case_e.lastChild().toElement().attribute("G").toInt();
            b = case_e.lastChild().toElement().attribute("B").toInt();
            a = case_e.lastChild().toElement().attribute("A").toInt();
            item->set_color(r, g, b, a);
            this->setItem(row, column, item);
        }
        QTableWidgetItem* item = this->takeItem(row, columnCount() - 1);
        item->setText(line.lastChild().toElement().text());
        this->setItem(row, columnCount() - 1, item);
    }
}

void ChordTableWidget::to_xml(QString xml_file)
{
    QDomDocument doc("XGRID");
    QDomElement chord_grid = doc.createElement("chord_grid");
    chord_grid.setAttribute("lines_number", this->rowCount());
    doc.appendChild(chord_grid);
    int red = -1, g = -1, b = -1, a = -1;
    for (int r = 0 ; r < this->rowCount() ; r++)
    {
        QDomElement line = doc.createElement("line");
        line.setAttribute("id", r);
        for (int c = 0 ; c < this->columnCount() - 1 ; c ++)
        {
            QDomElement case_node = doc.createElement("case");
            case_node.setAttribute("id", c);
            QDomElement chord = doc.createElement("chord");
            chord.setAttribute("name", this->item(r, c)->text());
            case_node.appendChild(chord);
            QDomElement color = doc.createElement("color");
            ((CaseItem*) this->item(r, c))->get_color()->getRgb(&red, &g, &b, &a);
            color.setAttribute("G", g);
            color.setAttribute("R", red);
            color.setAttribute("A", a);
            color.setAttribute("B", b);
            case_node.appendChild(color);
            line.appendChild(case_node);
        }
        QDomElement annotation = doc.createElement("annotation");
        QDomText content = doc.createTextNode(this->item(r, this->columnCount() - 1)->text());
        annotation.appendChild(content);
        line.appendChild(annotation);
        chord_grid.appendChild(line);
    }
    QFile file(xml_file);
    file.open(QIODevice::WriteOnly);
    QTextStream ts(&file);
    ts << doc.toString();
    std::cerr << doc.toString().toStdString();
    file.close();
}

bool ChordTableWidget::is_selection_empty()
{
    if (this->selectedIndexes().isEmpty())
        return true;
    return false;
}

bool ChordTableWidget::is_row_selected()
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (ranges.isEmpty())
        return false;
    for (QList<QTableWidgetSelectionRange>::Iterator it = ranges.begin() ; it != ranges.end() ; it ++)
        if ((*it).leftColumn() != 0 || (*it).rightColumn() != this->columnCount() - 1)
            return false;
    return true;
}

QList<QList<int>*> ChordTableWidget::rows_selected()
{
    QList<QList<int>*> range_rows;
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    for (QList<QTableWidgetSelectionRange>::Iterator it = ranges.begin() ; it != ranges.end() ; it ++)
        if ((*it).leftColumn() == 0 && (*it).rightColumn() == this->columnCount() - 1)
        {
            QList<int>* rows = new QList<int>();
            for (int r = (*it).topRow() ; r <= (*it).bottomRow() ; r ++)
                rows->push_back(r);
            range_rows.push_back(rows);
        }
    return range_rows;
}

void ChordTableWidget::insert_row(int pos, int num)
{
    for (int i = 0 ; i < num ; i ++)
    {
        this->insertRow(pos);
        for (int c = 0 ; c < this->columnCount() - 1 ; c ++)
        {
            this->setItem(pos, c, new CaseItem());
            this->setColumnWidth(c, 60);
        }
        this->setRowHeight(pos, 40);
        this->setItem(pos, this->columnCount() - 1, new QTableWidgetItem());
    }
}

void ChordTableWidget::fill_selection(QTreeWidgetItem* chord, int column)
{
    if (chord->childCount() == 0)
    {
        QModelIndexList index_list = this->selectedIndexes();
        for (QModelIndexList::Iterator it = index_list.begin() ; it != index_list.end() ; it ++)
            if ((*it).column() != this->columnCount() - 1)
            {
                CaseItem* item = (CaseItem*) this->takeItem((*it).row(), (*it).column());
                item->set_chord(chord->text(column));
                item->set_color(0, 200, 100, 150);
                this->setItem((*it).row(), (*it).column(), item);
            }
    }
}

QList<int> ChordTableWidget::expand_list(QList<QList<int>*> list)
{
    QList<int> result;
    for (QList<QList<int>*>::Iterator it = list.begin() ; it != list.end() ; it ++)
        result.append(**it);
    return result;
}

void ChordTableWidget::insert_row()
{
    if (is_row_selected())
    {
        QList<int> rows = expand_list(rows_selected());
        for (QList<int>::Iterator it = rows.begin() ; it != rows.end() ; it ++)
            insert_row(*it);
    }
    else
        insert_row(this->rowCount());
}

void ChordTableWidget::delete_selected_row()
{
    if (is_row_selected())
    {
        QList<int> rows = expand_list(rows_selected());
        qSort(rows.begin(), rows.end(), qGreater<int>());
        for (QList<int>::Iterator it = rows.begin() ; it != rows.end() ; it ++)
            this->removeRow(*it);
    }
}

void ChordTableWidget::copy_down_rows()
{
    if (is_row_selected())
    {
        QList<QList<int>*> ranges = rows_selected();
        int count = 0;
        QList<int>::Iterator i;
        for (QList<QList<int>*>::Iterator it = ranges.begin() ; it != ranges.end() ; it ++)
        {
            for (i = (**it).begin(), count = 1 ; i != (**it).end() ; i ++, count ++)
            {
                insert_row((**it).last() + count);
                for (int c = 0 ; c < this->columnCount() ; c ++)
                    this->setItem((**it).last() + count, c, this->item(*i, c)->clone());
            }
        }
    }
}
