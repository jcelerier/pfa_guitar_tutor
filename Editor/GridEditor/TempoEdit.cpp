#include "TempoEdit.h"

TempoEdit::TempoEdit(QWidget *parent) :
	QSpinBox(parent)
{
	connect(this, SIGNAL(valueChanged(int)), SLOT(changed(int)), Qt::DirectConnection);
	has_changed = false;
}


void TempoEdit::changed(int)
{
	has_changed = true;
}

void TempoEdit::mousePressEvent ( QMouseEvent * event )
{
	QAbstractSpinBox::mousePressEvent(event);

	if(has_changed == true)
	{
		emit hasBeenClicked();
		has_changed = false;
	}
}

void TempoEdit::mouseReleaseEvent(QMouseEvent *event)
{
	QAbstractSpinBox::mouseReleaseEvent(event);
	emit hasBeenClicked();
}
