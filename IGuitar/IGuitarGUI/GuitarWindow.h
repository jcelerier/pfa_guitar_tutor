#ifndef GUITARWINDOW_H
#define GUITARWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMenu>
#include <QMainWindow>
#include <QMdiArea>
#include <string>
using namespace std;

class GuitarWindow : public QMainWindow
{
    public:
    GuitarWindow();

    //ajouter des QWidget dans la fenetre Centrale directement.
    //cette fenetre utilise un GridLayout pour la disposition des éléments
    QMdiArea fenetreCentrale;
    void setButtonsVisible(bool b);

    private:
    //attribus
    QPushButton *previous_button;
    QPushButton *next_button;
    QMenu *menuFichier;
    QMenu *menuEdition;
    QMenu *menuAffichage;
    //méthodes
    void initMenu();
    QPushButton* initButton(const char *imagePath);
};

#endif // GUITARWINDOW_H
