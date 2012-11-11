#include "mainwindow.h"


MainWindow::MainWindow()
{
    this->setFixedSize(800,550);
    edit_button = new QPushButton("Editer un morceau",this);
    edit_button->move(300,300);
    numerise_button = new QPushButton("Numeriser un morceau",this);
    numerise_button->move(300,400);
    create_button = new QPushButton("Creer un morceau",this);
    create_button->move(300,200);
    this->setWindowTitle("Menu");

    numerise= new NumeriseWindow();
    QObject::connect(numerise_button, SIGNAL(clicked()), this, SLOT(close()) );
    QObject::connect(numerise_button, SIGNAL(clicked()), numerise, SLOT(show()) );

    create = new CreateWindow();
    QObject::connect(create_button, SIGNAL(clicked()), this, SLOT(close()) );
    QObject::connect(create_button, SIGNAL(clicked()), create, SLOT(show()) );

    edit = new EditWindow();
    QObject::connect(edit_button, SIGNAL(clicked()), this, SLOT(close()) );
    QObject::connect(edit_button, SIGNAL(clicked()), edit, SLOT(show()) );

    this->show();
}

MainWindow::~MainWindow(){
    delete edit_button;
    delete numerise_button;
    delete create_button;

}


