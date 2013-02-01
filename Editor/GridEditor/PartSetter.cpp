#include "PartSetter.h"
#include "ui_PartSetter.h"
#include "ChordTableWidget.h"

PartSetter::PartSetter(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PartSetter)
{
	ui->setupUi(this);
    ui->beginning->setDisplayFormat("m:ss:zzz");
    ui->beginning->setTime(QTime(0,0));
    ui->partEdit->setText("");
}

PartSetter::~PartSetter()
{
	delete ui;
}

void PartSetter::showDialogModal()
{
	// on affiche la boite de dialogue de facon modale
	setModal(true);
	show();
}

void PartSetter::initPart(QString part)
{
    if(part == "")
        ui->isPart->setChecked(false);
    ui->partEdit->setText(part);
}

void PartSetter::initBeginning(QTime t)
{
    ui->beginning->setTime(t);
}

void PartSetter::accept()
{
    if(ui->isPart->isChecked() && ui->partEdit->text() != "")
        ((ChordTableWidget*) parent())->setCasePart(ui->partEdit->text());
    else if(ui->isPart->isChecked() && !ui->isPart->isEnabled() && ui->partEdit->text() == ""){
        QMessageBox::information(this, tr("First part"), tr("You must give a part name to the first cell."));
        return;
    }
    else
        ((ChordTableWidget*) parent())->removeCasePart();
    ((ChordTableWidget*) parent())->setCaseBeginning(ui->beginning->time());
    hide();
    ((ChordTableWidget*) parent())->checkBeginningTimes();
}

void PartSetter::setEnabledPartEdit(int state)
{
    ui->labelPartEdit->setEnabled(state);
    ui->partEdit->setEnabled(state);
}

void PartSetter::setPartEditable(bool editable)
{
    ui->isPart->setEnabled(editable);
    ui->isPart->setChecked(true);
}
