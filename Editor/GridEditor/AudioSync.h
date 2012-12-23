#ifndef AUDIOSYNC_H
#define AUDIOSYNC_H

#include <QtGui>

#define TIMER_BEGGINING 0
#define TIMER_END       1
#define TIMER_BAR       2

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

    void setBarTimer(const QTime);
    void setBegginingTimer(const QTime);
    void setEndTimer(const QTime);
private slots:
    void emitSignalTimer();
signals:
    void refreshTimer(int);
};

#endif // AUDIOSYNC_H
