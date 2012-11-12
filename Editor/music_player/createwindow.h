#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include<QApplication>
#include"player.h"
#include"../chords_grid/CaseItem.h"
#include"../chords_grid/ChordTableWidget.h"




class CreateWindow : public QWidget
{
    Q_OBJECT

private:
    Player *player;
    //ChordTableWidget *chordTable;
    QGridLayout *layout;

public:
    CreateWindow();

public slots:
    void exportDataToFile();
};

#endif // CREATEWINDOW_H
