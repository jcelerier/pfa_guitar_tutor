/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 14/04/12
*/

#include "CaseItem.h"

CaseItem::CaseItem() :  QTableWidgetItem(), color(new QColor())
{
    color->setRgb(255, 255, 255);
    this->setBackgroundColor(color->toRgb());
    this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    this->setTextAlignment(Qt::AlignCenter);
}

CaseItem::CaseItem(const QTableWidgetItem& item) : QTableWidgetItem(item), color(new QColor(item.background().color()))
{
}

CaseItem::~CaseItem()
{
    delete color;
}

void CaseItem::set_color(int r, int g, int b, int a = 255)
{
    color->setRgb(r, g, b, a);
    this->setBackgroundColor(color->toRgb());
}

QColor* CaseItem::get_color() const
{
    return color;
}

void CaseItem::set_chord(QString chord)
{
    this->setText(chord);
}

QString CaseItem::get_chord() const
{
    return this->text();
}

CaseItem* CaseItem::clone() const
{
    QTableWidgetItem* item = QTableWidgetItem::clone();
    return new CaseItem((const QTableWidgetItem&) *item);
}
