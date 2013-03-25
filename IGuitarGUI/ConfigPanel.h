#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QWidget>
#include "Configuration.h"

namespace Ui {
class ConfigPanel;
}

/**
 * @brief Affiche un widget permettant le réglage de certains paramètres du player.
 */
class ConfigPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfigPanel(bool isLoopingActive = true, int difficulty = PERCENT_TO_VALIDATE_EASY, int continueMode = PAUSE_TO_SAME_TIME, QWidget *parent = 0);
    ~ConfigPanel();
    int getDifficulty() const;
    bool isLoopingActive() const;
    int getContinueMode() const;
public slots:
    void saveData();
signals:
    void configChanged(bool, int, int);
private:
    Ui::ConfigPanel *ui;
    bool m_isLoopingActive;
    int m_difficulty;
    int m_continueMode;
};

#endif // CONFIGPANEL_H
