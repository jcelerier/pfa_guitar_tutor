#ifndef TEMPOEDIT_H
#define TEMPOEDIT_H

#include <QtWidgets/QSpinBox>

/**
 * @brief Enfant de la QSpinBox pour afficher en rouge si données invalide
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
		virtual void keyPressEvent(QKeyEvent *event);
	signals:
		/**
		 * @brief hasManuallyChanged est émis lorsqu'on clique ou appuie sur le clavier lorsque sur le widget
		 */
		void hasManuallyChanged();

	public slots:
		void changed(int);

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

#endif // TEMPOEDIT_H
