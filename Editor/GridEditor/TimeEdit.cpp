#include "TimeEdit.h"
#include <QDebug>

TimeEdit::TimeEdit(QWidget *parent) :
	QTimeEdit(QTime(0, 0, 0, 0), parent)
{
	badStyle = "QTimeEdit { background-color:#ffDDDD;color:#000000 }";

	connect(this, SIGNAL(timeChanged(QTime)), SLOT(changed(QTime)), Qt::DirectConnection);
	has_changed = false;

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


/**
 * @brief TimeEdit::mousePressEvent
 * @param event évènement souris
 *
 * Réimplémenté depuis QAbstractSpinBox
 */
void TimeEdit::mousePressEvent ( QMouseEvent * event )
{
	QAbstractSpinBox::mousePressEvent(event);

	if(has_changed == true)
	{
		emit hasBeenClicked();
		has_changed = false;
	}
}

/**
 * @brief TimeEdit::changed
 *
 * Slot appelé lorsque la valeur a changé
 */
void TimeEdit::changed(QTime)
{
	has_changed = true;
}
