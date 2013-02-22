#include "TimeEdit.h"

TimeEdit::TimeEdit(QWidget *parent) :
	QTimeEdit(parent)
{
	badStyle = "QTimeEdit { background-color:#ffDDDD;color:#000000 }";
}

/**
 * @brief TimeEdit::setBad
 *
 * Actions à exécuter lorsqu'un TimeEdit n'est pas correctement réglé
 * => coloriage en rouge
 */
void TimeEdit::setBad()
{
	if(this->isEnabled())
	{
		this->setStyleSheet(badStyle);
	}
}

/**
 * @brief TimeEdit::setGood
 *
 * Actions à exécuter lorsqu'un TimeEdit est correctement réglé
 * => supprime le rouge
 */
void TimeEdit::setGood()
{
	this->setStyleSheet("");
}
