#ifndef PARTSETTER_H
#define PARTSETTER_H

#include <QDialog>

namespace Ui {
class PartSetter;
}

class PartSetter : public QDialog
{
		Q_OBJECT
		
	public:
		explicit PartSetter(QWidget *parent = 0);
		~PartSetter();

	public slots:
		void showDialogModal();
		void setPart(QString part);
		void accept();
		
	private:
		Ui::PartSetter *ui;
		QString m_part;
};

#endif // PARTSETTER_H
