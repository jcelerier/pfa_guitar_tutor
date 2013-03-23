#include "ChordDictionary.h"
#include "ui_ChordDictionary.h"

#include <QDebug>

#define CHORDS_PER_LINE 5

/**
 * @brief ChordDictionary::ChordDictionary
 * @param chordList Liste des accords à mettre dans le dictionnaire
 * @param parent Widget parent
 *
 * Constructeur.
 */
ChordDictionary::ChordDictionary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChordDictionary)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    m_listSize = 0;
    m_labels = 0;
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
 * @brief ChordDictionary::load
 * @param track LogicalTrack dont les accords sont à placer dans le dictionnaire
 *
 * Charge les accords d'une track dans le dictionnaire (supprime les anciens accords).
 */
void ChordDictionary::load(LogicalTrack *track)
{
    //Suppression des anciens accords
    for(int i=0; i<m_listSize; i++)
        ui->layout->removeWidget(&m_labels[i]);
    if(m_labels != 0) {
        delete[] m_labels;
        m_labels = 0;
    }

    //Création de la liste des accords de la track
    TrackChord* chord = track->getPartTrackList()[0]->getTrackChordsList()[0];
    QStringList chordList;
    do
    {
        if(chord->getChord() != "n")
            chordList = chordList << chord->getChord();
    } while((chord = chord->next()) != 0);
    //Suppression des doublons
    chordList.removeDuplicates();
    m_listSize = chordList.size();

    //Placement des images associés aux accords dans le layout
    m_labels = new QLabel[m_listSize];
    for(int i=0; i<m_listSize; i++) {
        QString path = QString(":/chordsImg/" + chordList[i].replace(QChar('#'), "d") + ".png");
        (&m_labels[i])->setPixmap(path);
        ui->layout->addWidget((&m_labels[i]), i/CHORDS_PER_LINE, i%CHORDS_PER_LINE);
    }
}
