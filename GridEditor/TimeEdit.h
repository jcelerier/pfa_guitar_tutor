#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include <QtWidgets/QTimeEdit>

/**
 * @brief The TimeEdit class
 *
 * Héritage de la QTimeEdit pour afficher en rouge si invalide
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
		/**
		 * @brief hasBeenClicked est émis lorsqu'on clique sur le widget
		 */
		void hasBeenClicked();

	public slots:
		void changed(QTime);

	private:
		/**
		 * @brief m_hasChanged vrai si a changé (pour éviter les feedbacks)
		 */
		bool m_hasChanged;

		/**
		 * @brief m_badStyle CSS qui correspond à un état invalide
		 */
		QString m_badStyle; //TODO mérite de passer en static const


};

#endif // TIMEEDIT_H
