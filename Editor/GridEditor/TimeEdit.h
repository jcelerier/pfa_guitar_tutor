#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include <QTimeEdit>

class TimeEdit : public QTimeEdit
{
		Q_OBJECT
	public:
		explicit TimeEdit(QWidget *parent = 0);
		void setBad();
		void setGood();

	private:
		QString badStyle;
		
};

#endif // TIMEEDIT_H
