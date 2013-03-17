#include "ChordDictionary.h"
#include "ui_ChordDictionary.h"

#include <QDebug>

#define CHORDS_PER_LINE 3

/**
 * @brief ChordDictionary::ChordDictionary
 * @param chordList Liste des accords à mettre dans le dictionnaire
 * @param parent Widget parent
 *
 * Constructeur.
 */
ChordDictionary::ChordDictionary(QList<PlayerChord>* chordList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChordDictionary)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    initChordDictionary(chordList);
    connect(ui->closeButton, SIGNAL(pressed()), this, SLOT(hide()));
}

/**
 * @brief ChordDictionary::~ChordDictionary
 *
 * Destructeur.
 */
ChordDictionary::~ChordDictionary()
{
    delete ui;
}

/**
 * @brief ChordDictionary::initChordDictionary
 * @param chordList Liste des accords à mettre dans le dictionnaire
 *
 * Construit l'interface du dictionnaire d'accords en plaçant les images des accords.
 */
void ChordDictionary::initChordDictionary(QList<PlayerChord>* chordList)
{
    QList<PlayerChord>::iterator ite;
    QStringList chords;
    // Retrait des doublons
    for (ite = chordList->begin(); ite != chordList->end(); ++ite) {
        chords << ite->getName();
    }
    chords.removeDuplicates();
    m_listSize = chords.size();

    // Affichage des accords
    int numberOfLines = (chords.size()%CHORDS_PER_LINE == 0) ? chords.size()/CHORDS_PER_LINE : chords.size()/CHORDS_PER_LINE + 1;
    for(int i=0; i<numberOfLines; i++) {
        for(int j=0; j<CHORDS_PER_LINE; j++) {
            if(i*CHORDS_PER_LINE+j > chords.size()) //Fin de la liste
                return;
            QLabel *label = new QLabel();
            QString path = QString(":/chordsImg/" + chords[i*CHORDS_PER_LINE+j].replace(QChar('#'), "d") + ".png");
            label->setPixmap(path);
            ui->layout->addWidget(label, i, j);
        }
    }
}
