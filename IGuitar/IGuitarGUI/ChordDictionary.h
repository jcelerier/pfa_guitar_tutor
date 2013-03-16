#ifndef CHORDDICTIONARY_H
#define CHORDDICTIONARY_H

#include <QWidget>
#include "PlayerChord.h"

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
    explicit ChordDictionary(QList<PlayerChord> *chordList, QWidget *parent = 0);
    ~ChordDictionary();
    void initChordDictionary(QList<PlayerChord>* chordList);
private:
    Ui::ChordDictionary *ui;
    int listSize;
};

#endif // CHORDDICTIONARY_H