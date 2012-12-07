#include "dialog.hpp"
#include "dialog.ui"

#include <QApplication>
#include <QWidget>
#include <string>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem("lol");
}

Dialog::~Dialog()
{
    delete ui;
}
