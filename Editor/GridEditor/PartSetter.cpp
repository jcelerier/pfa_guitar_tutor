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

	//ui->pushButton->addAction(((ChordTableWidget*) parent)->m_playFromHere);
}

PartSetter::~PartSetter()
{
	delete ui;
}

/**
 * @brief PartSetter::showDialogModal
 *Affiche le dialogue
 */
void PartSetter::showDialogModal()
{
	// on affiche la boite de dialogue de facon modale
	setModal(true);
	show();
}

/**
 * @brief PartSetter::initPart
 * @param part Texte de la partie
 */
void PartSetter::setPart(QString part)
{
    if(part == "")
        ui->isPart->setChecked(false);
    ui->partEdit->setText(part);
}

/**
 * @brief PartSetter::setBeginning
 * @param t Temps du début
 */
void PartSetter::setBeginning(const QTime& t)
{
    ui->beginning->setTime(t);
}

/**
 * @brief PartSetter::accept
 *
 * Slot appelé lorsqu'on appuie sur Ok, effectue diverses vérifications et sauvegarde les données
 */
void PartSetter::accept()
{
    if(ui->isPart->isChecked() && ui->partEdit->text() != "")
	{
        ((ChordTableWidget*) parent())->setCasePart(ui->partEdit->text());
	}
	else if(ui->isPart->isChecked() && !ui->isPart->isEnabled() && ui->partEdit->text() == "")
	{
        QMessageBox::information(this, tr("First part"), tr("You must give a part name to the first cell."));
        return;
    }
    else
	{
        ((ChordTableWidget*) parent())->removeCasePart();
	}

	if(ui->updateFollowers->isChecked())
	{
		((ChordTableWidget*) parent())->setCaseAndFollowersBeginning(ui->beginning->time());
	}
	else
	{
		((ChordTableWidget*) parent())->setCaseBeginning(ui->beginning->time());
	}

	hide();
    ((ChordTableWidget*) parent())->checkBeginningTimes();
}

/**
 * @brief PartSetter::setEnabledPartEdit
 * @param state Vrai si on active les widgets, faux sinon
 *
 * Mets les widgets dans l'état state
 */
void PartSetter::setEnabledPartEdit(int state)
{
    ui->labelPartEdit->setEnabled(state);
    ui->partEdit->setEnabled(state);
}

/**
 * @brief PartSetter::setPartEditable
 * @param editable
 *
 * Rend la partie éditable
 */
void PartSetter::setPartEditable(bool editable)
{
    ui->isPart->setEnabled(editable);
    ui->isPart->setChecked(true);
}

/**
 * @brief PartSetter::on_pushButton_clicked
 * Active la lecture à partir du beginning de cette case, pour faire des réglages fins
 */
void PartSetter::on_pushButton_clicked()
{
	((ChordTableWidget*) parent())->playFromHere();
}
