#include "ConfigPanel.h"
#include "ui_ConfigPanel.h"

ConfigPanel::ConfigPanel(bool isLoopingActive, enum Difficulty difficulty, enum ContinueMode continueMode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigPanel),
    m_isLoopingActive(isLoopingActive),
    m_difficulty(difficulty),
    m_continueMode(continueMode)
{
    ui->setupUi(this);

    ui->continueType->addItem(tr("Continue immediatly"));
    ui->continueType->addItem(tr("Restart from the last chord"));
    ui->continueType->addItem(tr("Restart from the beginning of the part"));

    this->setWindowModality(Qt::WindowModal);
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
}
