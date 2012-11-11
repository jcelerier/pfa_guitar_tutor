#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include<QApplication>
#include"player.h"




class CreateWindow : public QWidget
{
    Q_OBJECT

private:
    Player *player;
    QGridLayout *layout;

public:
    CreateWindow();

public slots:
    void exportDataToFile();
};

#endif // CREATEWINDOW_H
