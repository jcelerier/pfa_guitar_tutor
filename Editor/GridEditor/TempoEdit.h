#ifndef TEMPOEDIT_H
#define TEMPOEDIT_H

#include <QtGui>
#include <QSpinBox>

class TempoEdit : public QSpinBox
{
		Q_OBJECT
	public:
		explicit TempoEdit(QWidget *parent = 0);
		void setBad();
		void setGood();

		virtual void mousePressEvent ( QMouseEvent * event );
		virtual void mouseReleaseEvent(QMouseEvent *event);

	signals:
		void hasBeenClicked();

	public slots:
		void changed(int);

	private:
		bool has_changed;
		QString badStyle; //mérite de passer en static const
		
};

#endif // TEMPOEDIT_H
