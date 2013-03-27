#include "TimeEdit.h"


/**
 * @brief TimeEdit::TimeEdit
 * @param parent Widget parent
 */
TimeEdit::TimeEdit(QWidget *parent) :
	QTimeEdit(QTime(0, 0, 0, 0), parent)
{
	m_badStyle = "QTimeEdit { background-color:#ffDDDD;color:#000000 }";

	connect(this, SIGNAL(timeChanged(QTime)), SLOT(changed(QTime)), Qt::DirectConnection);
	m_hasChanged = false;

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
		this->setStyleSheet(m_badStyle);
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

	if(m_hasChanged == true)
	{
		emit hasBeenClicked();
		m_hasChanged = false;
	}
}

/**
 * @brief TimeEdit::changed
 *
 * Slot appelé lorsque la valeur a changé
 */
void TimeEdit::changed(QTime)
{
	m_hasChanged = true;
}
