#include "ConfigPanel.h"
#include "ui_ConfigPanel.h"

ConfigPanel::ConfigPanel(bool isLoopingActive, int difficulty, int continueMode, QWidget *parent) :
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
    case PERCENT_TO_VALIDATE_EASY:
        ui->easyMode->setChecked(true);
        break;
    case PERCENT_TO_VALIDATE_MEDIUM:
        ui->easyMode->setChecked(true);
        break;
    case PERCENT_TO_VALIDATE_HARD:
        ui->hardMode->setChecked(true);
        break;
    default:
        qDebug() << "Bad difficulty";
        break;
    }

    this->setWindowModality(Qt::WindowModal);
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
}

int ConfigPanel::getDifficulty() const {
    return m_difficulty;
}

int ConfigPanel::getContinueMode() const {
    return m_continueMode;
}

bool ConfigPanel::isLoopingActive() const {
    return m_isLoopingActive;
}

void ConfigPanel::saveData()
{
    m_difficulty = (ui->easyMode->isChecked()) ? PERCENT_TO_VALIDATE_EASY :
                                                 ((ui->mediumMode->isChecked()) ? PERCENT_TO_VALIDATE_MEDIUM : PERCENT_TO_VALIDATE_HARD);
    m_continueMode = ui->continueType->currentIndex();
    m_isLoopingActive = ui->activateLoop->isChecked();

    hide();

    emit configChanged(m_isLoopingActive, m_difficulty, m_continueMode);
}
