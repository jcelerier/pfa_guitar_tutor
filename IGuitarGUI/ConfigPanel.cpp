#include "ConfigPanel.h"
#include "ui_ConfigPanel.h"

/**
 * @brief ConfigPanel::ConfigPanel
 * @param isLoopingActive Valeur par défaut du bouclage
 * @param difficulty Valeur par défaut de la difficulté
 * @param continueMode Valeur par défaut du mode de reprise
 * @param parent Widget parent
 *
 * Constructeur.
 */
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

/**
 * @brief ConfigPanel::~ConfigPanel
 *
 * Destructeur.
 */
ConfigPanel::~ConfigPanel()
{
    delete ui;
}

/**
 * @brief ConfigPanel::getDifficulty
 * @return Le niveau de difficulté en pourcentage pour valider une note.
 */
int ConfigPanel::getDifficulty() const {
    return m_difficulty;
}

/**
 * @brief ConfigPanel::getContinueMode
 * @return Le mode de reprise après pause.
 */
int ConfigPanel::getContinueMode() const {
    return m_continueMode;
}

/**
 * @brief ConfigPanel::isLoopingActive
 * @return Vrai si et seulement si le bouclage entre parties doit etre activé.
 */
bool ConfigPanel::isLoopingActive() const {
    return m_isLoopingActive;
}

/**
 * @brief ConfigPanel::saveData
 *
 * Sauvegarde les données de l'interface dans les attributs (après clic sur le bouton "Ok").
 */
void ConfigPanel::saveData()
{
    m_difficulty = (ui->easyMode->isChecked()) ? PERCENT_TO_VALIDATE_EASY :
                                                 ((ui->mediumMode->isChecked()) ? PERCENT_TO_VALIDATE_MEDIUM : PERCENT_TO_VALIDATE_HARD);
    m_continueMode = ui->continueType->currentIndex();
    m_isLoopingActive = ui->activateLoop->isChecked();

    hide();

    emit configChanged(m_isLoopingActive, m_difficulty, m_continueMode);
}
