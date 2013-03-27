#ifndef TEMPOEDIT_H
#define TEMPOEDIT_H

#include <QtGui>
#include <QtWidgets/QSpinBox>

/**
 * @brief The TempoEdit class
 *
 * Héritage de la QSpinBox pour afficher en rouge si invalide
 */
class TempoEdit : public QSpinBox
{
		Q_OBJECT
	public:
		explicit TempoEdit(QWidget *parent = 0);
		void setBad();
		void setGood();

		virtual void mousePressEvent(QMouseEvent * event);
		virtual void mouseReleaseEvent(QMouseEvent *event);

	signals:
		void hasBeenClicked();

	public slots:
		void changed(int);

	private:
		bool m_hasChanged;
		QString m_badStyle; //mérite de passer en static const

};

#endif // TEMPOEDIT_H
