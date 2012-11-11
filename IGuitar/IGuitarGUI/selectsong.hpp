#ifndef SELECTSONG_HPP
#define SELECTSONG_HPP

#include <QDialog>
#include <QComboBox>

#include "GuitarWindow.h"

class SelectSong : public GuitarWindow
{
public:
    SelectSong(QString &);

private:
    //attributs
    QComboBox combobox;
};

#endif // SELECTSONG_HPP
