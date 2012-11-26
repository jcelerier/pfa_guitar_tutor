#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include<QApplication>
#include"player.h"
#include"../GridEditor/CaseItem.h"
#include"../GridEditor/ChordTableWidget.h"




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
