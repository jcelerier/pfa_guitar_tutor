#ifndef AUDIOSYNC_H
#define AUDIOSYNC_H

#include <QWidget>
#include <QtGui>
#include <QGridLayout>
#include <QTimeEdit>
#include <QToolButton>
#include <QPushButton>
#include <QTime>
#include <QLabel>
#define TIMER_BEGINNING 0
#define TIMER_END       1
#define TIMER_BAR       2

class AudioSync : public QWidget
{
    Q_OBJECT

    QGridLayout* layout;
    QTimeEdit *beginning, *end, *bar;
    QToolButton* bbeginning, *bend, *bbar;
	QLabel *lbeginning, *lend, *lbar, *lbpm;
	QSpinBox *bpm;
	QPushButton *sendButton;

public:
    AudioSync();
    ~AudioSync();
    void activeButtons(bool active);

    void setBarTimer(const QTime);
    void setBegginingTimer(const QTime);
    void setEndTimer(const QTime);

public slots:
	void sendData();

private slots:
    void emitSignalTimer();
	void beginningChanged(QTime);
	void barChanged(QTime);
	void endChanged(QTime);
	void tempoChanged(int tempo);

signals:
    void refreshTimer(int);
	void sendTimers(QTime, QTime, QTime);
	void sendTimer(int, QTime);
};

#endif // AUDIOSYNC_H
