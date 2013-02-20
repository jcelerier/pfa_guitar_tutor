#include "TimeEdit.h"

TimeEdit::TimeEdit(QWidget *parent) :
	QTimeEdit(parent)
{
	badStyle = "QTimeEdit { background-color:#ffDDDD;color:#000000 }";
}

void TimeEdit::setBad()
{
	if(this->isEnabled())
	{
		this->setStyleSheet(badStyle);
	}
}

void TimeEdit::setGood()
{
	this->setStyleSheet("");
}
