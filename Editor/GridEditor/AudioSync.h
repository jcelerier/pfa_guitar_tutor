#ifndef AUDIOSYNC_H
#define AUDIOSYNC_H

#include <QtGui>

class AudioSync : public QWidget
{
    Q_OBJECT

    QGridLayout* layout;
    QTimeEdit *beggining, *end, *bar;
    QToolButton* bbeggining, *bend, *bbar;
public:
    AudioSync();
    ~AudioSync();
    void activeButtons(bool active);
};

#endif // AUDIOSYNC_H
