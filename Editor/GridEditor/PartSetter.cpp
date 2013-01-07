#include "PartSetter.h"
#include "ui_PartSetter.h"
#include "ChordTableWidget.h"

PartSetter::PartSetter(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PartSetter)
{
	ui->setupUi(this);

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

//pour que ça soit actualisé en fonction de la case ou on clique,
//il faut faire hériter le slot onClick (ou autre) de ChordTableWidget
//pour qu'il fasse un PartSetter::setPart(CaseItem::getPart(currentItem))
void PartSetter::setPart(QString part)
{
    m_part = part;
}

void PartSetter::accept()
{
    if(ui->isPart->isChecked())
        ((ChordTableWidget*) parent())->setCasePart(m_part);
    else
        ((ChordTableWidget*) parent())->removeCasePart();
    hide();
}

void PartSetter::setEnabledPartEdit(int state)
{
    ui->labelPartEdit->setEnabled(state);
    ui->partEdit->setEnabled(state);
}
