#ifndef PARTSETTER_H
#define PARTSETTER_H

#include <QDialog>
#include <QStringList>
#include <QTime>

namespace Ui {
class PartSetter;
}

class PartSetter : public QDialog
{
		Q_OBJECT
		
	public:
    explicit PartSetter(QWidget *parent = 0);
		~PartSetter();
    void setBeginning(const QTime &t);
    void initPart(QString part);
    void initBeginning(QTime t);
    void showDialogModal();
public slots:

		void accept();
		void setEnabledPartEdit(bool state);
		
	private:
		Ui::PartSetter *ui;
};

#endif // PARTSETTER_H
