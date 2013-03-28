#ifndef PARTSETTER_H
#define PARTSETTER_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QMessageBox>
#include <QTime>

namespace Ui {
class PartSetter;
}

/**
 * @brief Fenêtre qui s'affiche lors d'un clic droit sur une case
 */
class PartSetter : public QDialog
{
		Q_OBJECT

	public:
		explicit PartSetter(QWidget *parent = 0);
		~PartSetter();
		void setBeginning(const QTime &t);
		void setPart(QString part);

		void showDialogModal();
		void setPartEditable(bool editable);

	public slots:
		void accept();
		void setEnabledPartEdit(int state);

	private slots:
		void on_pushButton_clicked();

	private:
		/**
		 * @brief ui Vue de cette classe
		 */
		Ui::PartSetter *ui;
};

#endif // PARTSETTER_H
