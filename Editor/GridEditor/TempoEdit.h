#ifndef TEMPOEDIT_H
#define TEMPOEDIT_H

#include <QtGui>
#include <QSpinBox>

class TempoEdit : public QSpinBox
{
		Q_OBJECT
	public:
		explicit TempoEdit(QWidget *parent = 0);
		virtual void mousePressEvent ( QMouseEvent * event );
		virtual void mouseReleaseEvent(QMouseEvent *event);

	signals:
		void hasBeenClicked();

	public slots:
		void changed(int);

	private:
		bool has_changed;
		
};

#endif // TEMPOEDIT_H
