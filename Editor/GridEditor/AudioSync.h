#ifndef AUDIOSYNC_H
#define AUDIOSYNC_H

#include <QtGui>

class AudioSync : public QWidget
{
    Q_OBJECT

    QLineEdit *beggining, *end, *bar;
public:
    AudioSync();
    ~AudioSync();
};

#endif // AUDIOSYNC_H
