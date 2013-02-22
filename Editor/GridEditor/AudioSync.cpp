#include "AudioSync.h"
#include "Util.hpp"
#include <QMessageBox>
#include <QDebug>



/**
 * @brief AudioSync::AudioSync
 * Constructeur des boutons de synchronisation audio
 */
AudioSync::AudioSync()
{
    layout = new QGridLayout();
	beginning = new TimeEdit();
	end = new TimeEdit();
	bar = new TimeEdit();
	bpm = new QSpinBox();


    lbeginning = new QLabel(tr("Beginning"));
    lend = new QLabel(tr("End"));
    lbar = new QLabel(tr("Bar"));
	lbpm = new QLabel(tr("BPM"));

    bbeginning = new QToolButton();
    bend = new QToolButton();
    bbar = new QToolButton();

    sendButton = new QPushButton(tr("Compute"));

    bbeginning->setIcon(QIcon("icons/timer.png"));
    bend->setIcon(QIcon("icons/timer.png"));
    bbar->setIcon(QIcon("icons/timer.png"));

	beginning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    activeButtons(false);

	bpm->setMinimum(10);
	bpm->setMaximum(250);
	beginning->setDisplayFormat("m:ss:zzz");
	end->setDisplayFormat("m:ss:zzz");
	end->setBad();
	bar->setDisplayFormat("m:ss:zzz");
	bar->setBad();

	layout->addWidget(lbeginning, 0, 0);
	layout->addWidget(beginning, 0, 1);
    layout->addWidget(bbeginning, 0, 2);

	layout->addWidget(lbar, 1, 0);
	layout->addWidget(bar, 1, 1);
	layout->addWidget(bbar, 1, 2);

	layout->addWidget(lend, 2, 0);
	layout->addWidget(end, 2, 1);
	layout->addWidget(bend, 2, 2);


	layout->addWidget(sendButton, 3, 0);

	layout->addWidget(lbpm, 4, 0);
	layout->addWidget(bpm, 4, 1);

    setLayout(layout);

    connect(bbeginning, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bend, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
    connect(bbar, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));

	connect(beginning, SIGNAL(timeChanged(QTime)), this, SLOT(beginningChanged(QTime)));
	connect(bar, SIGNAL(timeChanged(QTime)), this, SLOT(barChanged(QTime)));
	connect(end, SIGNAL(timeChanged(QTime)), this, SLOT(endChanged(QTime)));

	connect(bpm, SIGNAL(valueChanged(int)), this, SLOT(tempoChanged(int)));

	connect(sendButton, SIGNAL(clicked()), this, SLOT(sendData()));
}

AudioSync::~AudioSync()
{

	delete bpm;
	delete beginning;
    delete end;
    delete bar;

	delete layout;

    delete bbeginning;
    delete bend;
    delete bbar;

	delete lbpm;
    delete lend;
    delete lbeginning;
    delete lbar;


}

/**
 * @brief AudioSync::activeButtons
 * @param active Vrai si les boutons doivent être activés, faux sinon
 *
 * Active l'interface AudioSync (rend les éléments clickables)
 */
void AudioSync::activeButtons(bool active)
{
	beginning->setEnabled(active);
    end->setEnabled(active);
    bar->setEnabled(active);

	if(active)
	{
		endChanged(end->time());
		beginningChanged(beginning->time());
		barChanged(bar->time());
	}
}

/**
 * @brief AudioSync::setBegginingTimer
 * @param t Temps du début
 *
 * Appelé principalement lors de la pression du petit bouton à la droite du TimeEdit
 */
void AudioSync::setBegginingTimer(const QTime t)
{
	beginning->setTime(t);
}

/**
 * @brief AudioSync::setEndTimer
 * @param t Temps de la fin

 * Appelé principalement lors de la pression du petit bouton à la droite du TimeEdit
 */
void AudioSync::setEndTimer(const QTime t)
{
    end->setTime(t);
}

/**
 * @brief AudioSync::setBarTimer
 * @param t Temps de la mesure
 * Appelé principalement lors de la pression du petit bouton à la droite du TimeEdit
 */
void AudioSync::setBarTimer(const QTime t)
{
    bar->setTime(t);
}

/**
 * @brief AudioSync::beginningChanged
 * @param t Temps du début
 *
 * Utilisé principalement pour modifier la waveform en temps réel
 */
void AudioSync::beginningChanged(QTime t)
{
	checkTimes();
	emit sendTimer(TIMER_BEGINNING, t);
}

/**
 * @brief AudioSync::barChanged
 * @param t Temps de la mesure
 * Utilisé principalement pour modifier la waveform en temps réel
 */
void AudioSync::barChanged(QTime t)
{
	checkTimes();
	int tps = TimeToMsec(t) - TimeToMsec(beginning->time());
	if(tps > 0)
	{
		int tempo = 240000 / tps ;

		if(tempo > 10 && tempo < 250 && tempo != bpm->value())
			bpm->setValue(tempo);
	}


	emit sendTimer(TIMER_BAR, t);
}

/**
 * @brief AudioSync::endChanged
 * @param t Temps de la fin
 * Utilisé principalement pour modifier la waveform en temps réel
 */
void AudioSync::endChanged(QTime t)
{
	checkTimes();
	emit sendTimer(TIMER_END, t);
}

/**
 * @brief AudioSync::checkTimes
 *
 * Vérifie qu'on ait à tout instant beginning < bar < end,
 * sinon offre une indication visuelle de l'incorrection des données entrées
 */
void AudioSync::checkTimes()
{
	long long tps = TimeToMsec(end->time()) - TimeToMsec(beginning->time());
	if(tps > 0)
	{
		end->setGood();
	}
	else
	{
		end->setBad();
	}

	long long tps1 = TimeToMsec(end->time()) - TimeToMsec(bar->time());
	long long tps2 = TimeToMsec(bar->time()) - TimeToMsec(beginning->time());
	if(tps1 > 0 && tps2 > 0)
	{
		bar->setGood();
	}
	else
	{
		bar->setBad();
	}
}

/**
 * @brief AudioSync::tempoChanged
 * @param tempo
 *
 * Slot utilisé lors du changement du tempo, mets à jour le TimeEdit en conséquence
 */
void AudioSync::tempoChanged(int tempo)
{
	QTime bar_time = MsecToTime(240000 / tempo + TimeToMsec(beginning->time()));

	bar->setTime(bar_time);
}

/**
 * @brief AudioSync::emitSignalTimer
 *
 */
void AudioSync::emitSignalTimer()
{
    if(bbeginning->isDown())
		emit refreshTimer(TIMER_BEGINNING);
    else if(bend->isDown())
        emit refreshTimer(TIMER_END);
    else if(bbar->isDown())
        emit refreshTimer(TIMER_BAR);
}

/**
 * @brief AudioSync::sendData
 *
 * Envoie les données à GridEditor
 */
void AudioSync::sendData()
{
    if(beginning->time().isValid() && bar->time().isValid() && end->time().isValid())
	{
		emit sendTimers(beginning->time(), bar->time(), end->time());
	}
    else
		QMessageBox::information(this, tr("Your job is not done yet"), tr("You have not set the three timers yet."));
}
