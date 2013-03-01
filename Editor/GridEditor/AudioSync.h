#ifndef AUDIOSYNC_H
#define AUDIOSYNC_H

#include <QtWidgets/QWidget>
#include <QtGui>
#include <QtWidgets/QGridLayout>
#include "TimeEdit.h"
#include "TempoEdit.h"
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QTime>
#include <QtWidgets/QLabel>
#define TIMER_BEGINNING 0
#define TIMER_END       1
#define TIMER_BAR       2
#define TIMER_PLAY		3
class TimeEdit;

/**
 * @brief Interface de synchronisation audio
 */
class AudioSync : public QWidget
{
    Q_OBJECT

    QGridLayout* layout;
	TimeEdit *beginning, *end, *bar;
    QToolButton* bbeginning, *bend, *bbar;
	QLabel *lbeginning, *lend, *lbar, *lbpm;
	TempoEdit *bpm;
	QPushButton *sendButton;
	QTime m_tempBarTime;


public:
	AudioSync(QWidget* parent = 0);
    ~AudioSync();
	void activeButtons(bool active);

    void setBarTimer(const QTime);
    void setBegginingTimer(const QTime);
    void setEndTimer(const QTime);
	void checkTimes();
	void updateTempo();

public slots:
	void sendData();
	void tempoClicked();
	void recvTimer(int, QTime);

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
