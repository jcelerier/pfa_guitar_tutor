#include "ConfigPanel.h"
#include "ui_ConfigPanel.h"

ConfigPanel::ConfigPanel(bool isLoopingActive, Difficulty difficulty, ContinueMode continueMode, QWidget *parent) :
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
    ui->continueType->setCurrentIndex(m_continueMode);

    ui->activateLoop->setChecked(m_isLoopingActive);

    switch(m_difficulty){
    case EASY:
        ui->easyMode->setChecked(true);
        break;
    case MEDIUM:
        ui->easyMode->setChecked(true);
        break;
    case HARD:
        ui->hardMode->setChecked(true);
    }

    this->setWindowModality(Qt::WindowModal);
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
}

Difficulty ConfigPanel::getDifficulty() const {
    return m_difficulty;
}

ContinueMode ConfigPanel::getContinueMode() const {
    return m_continueMode;
}

bool ConfigPanel::isLoopingActive() const {
    return m_isLoopingActive;
}

void ConfigPanel::saveData()
{
    m_difficulty = (ui->easyMode->isChecked()) ? EASY : ((ui->mediumMode->isChecked()) ? MEDIUM : HARD);
    m_continueMode = (ui->continueType->currentIndex() == 0) ? DIRECT : ((ui->continueType->currentIndex() == 1) ? CHORD : PART);
    m_isLoopingActive = ui->activateLoop->isChecked();
}
