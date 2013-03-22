#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#include <QWidget>

enum Difficulty { EASY, MEDIUM, HARD };
enum ContinueMode { DIRECT, CHORD, PART };

namespace Ui {
class ConfigPanel;
}

class ConfigPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfigPanel(bool isLoopingActive = true, enum Difficulty difficulty = EASY, enum ContinueMode continueMode = DIRECT, QWidget *parent = 0);
    ~ConfigPanel();
    
private:
    Ui::ConfigPanel *ui;
    bool m_isLoopingActive;
    enum Difficulty m_difficulty;
    enum ContinueMode m_continueMode;
};

#endif // CONFIGPANEL_H
