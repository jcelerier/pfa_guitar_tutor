#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include <QTimeEdit>

/**
 * @brief Sous-classe de QTimeEdit qui permet de changer la couleur
 */
class TimeEdit : public QTimeEdit
{
		Q_OBJECT
	public:
		explicit TimeEdit(QWidget *parent = 0);
		void setBad();
		void setGood();

		virtual void mousePressEvent ( QMouseEvent * event );

	signals:
		void hasBeenClicked();

	public slots:
		void changed(QTime);

	private:
		bool has_changed;
		QString badStyle;

		
};

#endif // TIMEEDIT_H
