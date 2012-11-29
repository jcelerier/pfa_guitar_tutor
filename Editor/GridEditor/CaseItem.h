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

class CaseItem : public QTableWidgetItem
{
    QColor* color;

public:
    CaseItem();
    CaseItem(const QTableWidgetItem& item);
    ~CaseItem();
    void set_color(int r, int g, int b, int);
    void set_chord(QString cw);
    QColor* get_color() const;
    QString get_chord() const;
    CaseItem* clone() const;
};

#endif // CASEITEM_H
