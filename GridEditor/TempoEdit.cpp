#include "TempoEdit.h"

TempoEdit::TempoEdit(QWidget *parent) :
	QSpinBox(parent)
{
	m_badStyle = "QSpinBox { background-color:#ffDDDD;color:#000000 }";

	connect(this, SIGNAL(valueChanged(int)), SLOT(changed(int)), Qt::DirectConnection);
	m_hasChanged = false;
}

/**
 * @brief TempoEdit::setBad
 *
 * Actions à exécuter lorsqu'un TempoEdit n'est pas correctement réglé
 * => coloriage en rouge
 */
void TempoEdit::setBad()
{
	if(this->isEnabled())
	{
		this->setStyleSheet(m_badStyle);
	}
}

/**
 * @brief TimeEdit::setGood
 *
 * Actions à exécuter lorsqu'un TempoEdit est correctement réglé
 * => supprime le rouge
 */
void TempoEdit::setGood()
{
	this->setStyleSheet("");
}


/**
 * @brief TempoEdit::changed
 *
 * Slot appelé lorsque la valeur a changé
 */
void TempoEdit::changed(int)
{
	m_hasChanged = true;
}

/**
 * @brief TempoEdit::mousePressEvent
 * @param event évènement souris
 *
 * Réimplémenté depuis QAbstractSpinBox
 */
void TempoEdit::mousePressEvent ( QMouseEvent * event )
{
	QAbstractSpinBox::mousePressEvent(event);

	if(m_hasChanged == true)
	{
		emit hasBeenClicked();
		m_hasChanged = false;
	}
}

/**
 * @brief TempoEdit::mouseReleaseEvent
 * @param event évènement souris
 *
 * Réimplémenté depuis QAbstractSpinBox
 */
void TempoEdit::mouseReleaseEvent(QMouseEvent *event)
{
	QAbstractSpinBox::mouseReleaseEvent(event);
	emit hasBeenClicked();
}
