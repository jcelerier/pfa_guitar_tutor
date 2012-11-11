/**
*Classe fournissant un format pour toutes les pages.
*Elle reprend la classe QMainWindow de Qt et red�finie certains attributs.
*On donne la possiblit� d'afficher ou non deux boutons en bas de la page,
*pour avancer ou reculer � une autre page
*@ Author: nretrain
*@ Date : 10/02/2012
*/

#include "GuitarWindow.h"
#include <stdio.h>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMenuItem>


GuitarWindow::GuitarWindow() : QMainWindow()
{
    setMinimumSize(800, 600);
    QMdiArea *fenetreCentrale = new QMdiArea;
    fenetreCentrale->setWindowTitle("GuitarTutor");
    initMenu();
    next_button=initButton("images/next.png");
    previous_button=initButton("images/previous.png");

    //positionne les boutons dans la fenetreCentrale
    QGridLayout *layout = new QGridLayout;
    QWidget *w= new QWidget();
    //Qwidget qui ne sert � rien, utilis� pour actualiser le layout et mettre les boutons
    //� leur vrai place (ie: en bas � gauche)
    layout->addWidget(next_button, 14, 15);
    layout->addWidget(previous_button, 14, 13);
    layout->addWidget(w,1,1,13,15);
    fenetreCentrale->setStyleSheet("QWidget { background-color: white; }");
    fenetreCentrale->setLayout(layout);
    setCentralWidget(fenetreCentrale);

  }
/**
  *Cr�er un bouton type pour la page principale.
  */

QPushButton* GuitarWindow::initButton(const char* imagePath)
{
    QPushButton *button = new QPushButton("");
    button->setFont(QFont("Comic Sans MS", 10));
    button->setCursor(Qt::PointingHandCursor);
    button->setIcon(QIcon(imagePath));
    button->setMaximumSize(60, 30);
    button->setStyleSheet("QPushButton { background-color: white; }");
    return button;
}

/**
  *Initialiser la menuBar du haut
  */
void GuitarWindow::initMenu()
{
    menuFichier = menuBar()->addMenu("&Fichier");
    QAction *actionQuitter = menuFichier->addAction("&Quitter");
    menuEdition = menuBar()->addMenu("&Edition");
    menuAffichage = menuBar()->addMenu("&Affichage");
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
}

/**
  *Permet d'afficher ou non les boutons suivant et pr�c�dent
  */
void GuitarWindow::setButtonsVisible(bool b)
{
    if(next_button!=NULL){
        next_button->setVisible(b);
    }
    if(previous_button!=NULL)
        previous_button->setVisible(b);
}
