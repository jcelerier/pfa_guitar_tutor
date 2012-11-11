#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QApplication>
#include<QPushButton>

#include "numerisewindow.h"
#include "createwindow.h"
#include "editwindow.h"


class MainWindow : public QWidget
{

private:
    QPushButton* edit_button;
    QPushButton* numerise_button;
    QPushButton* create_button;
    NumeriseWindow* numerise;
    CreateWindow* create;
    EditWindow* edit;


public:
    MainWindow();
    ~MainWindow();
    QPushButton* get_edit_button();
    QPushButton* get_numerise_button();
    QPushButton* get_create_button();

};

#endif // MAINWINDOW_H
