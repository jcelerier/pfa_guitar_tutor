#include "selectsong.hpp"
#include "getfilename.hpp"
#include <QDialog>
#include <QVariant>

SelectSong::SelectSong(QString &path)
{
    GuitarWindow * fenetre = new GuitarWindow();

    //QWidget * wid = new QWidget;
    QComboBox * combobox = new QComboBox();//(wid);
    GetFileName * gfn = new GetFileName();
    // recuperer les fichiers du dossier voulu et les ajouter dans la combobox
    const QStringList files = gfn->getFileName(path);
   // for (int i = 0; i<files->size(); i++) {
        combobox->addItems(files); // pb de type pour le addItem... un candidat serait addItem(const Qstring&, const Qvariant&)
   // }
    combobox->addItem("Test");
    combobox->addItem("Toto");
    combobox->show();

    //fenetre->fenetreCentrale.addSubWindow(combobox);
    //fenetre->show();
}
