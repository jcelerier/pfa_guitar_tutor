#ifndef CHORDDICTIONARY_H
#define CHORDDICTIONARY_H

#include <QWidget>
#include <Track/LogicalTrack.h>
#include <QLabel>

namespace Ui {
class ChordDictionary;
}

/**
 * @brief Interface du dictionnaire d'accords.
 */
class ChordDictionary : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChordDictionary(QWidget *parent = 0);
    ~ChordDictionary();
    void load(LogicalTrack* track);
private:
    Ui::ChordDictionary *ui;
    int m_listSize;
    QLabel* m_labels;
};

#endif // CHORDDICTIONARY_H
