#include "AudioSync.h"
#include <Util.hpp>
#include <QtWidgets/QMessageBox>
#include <QDebug>



/**
 * @brief AudioSync::AudioSync
 * Constructeur des boutons de synchronisation audio
 */
AudioSync::AudioSync(QWidget* parent) : QWidget(parent), m_timeSignature(4)
{
	layout = new QGridLayout();
	beginning = new TimeEdit();
	end = new TimeEdit();
	bar = new TimeEdit();
	bpm = new TempoEdit();

	lbeginning = new QLabel(tr("Beginning"));
	lend = new QLabel(tr("End"));
	lbar = new QLabel(tr("Bar"));
	lbpm = new QLabel(tr("BPM"));

	bbeginning = new QToolButton();
	bend = new QToolButton();
	bbar = new QToolButton();

	sendButton = new QPushButton(tr("Compute"));

	// Propriétés du GUI
	activeButtons(false);

	bbeginning->setIcon(QIcon(":/icons/timer.png"));
	bend->setIcon(QIcon(":/icons/timer.png"));
	bbar->setIcon(QIcon(":/icons/timer.png"));

	beginning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	bpm->setMinimum(10);
	bpm->setMaximum(250);
	bpm->setBad();

	beginning->setDisplayFormat("m:ss:zzz");

	end->setDisplayFormat("m:ss:zzz");
	end->setBad();

	bar->setDisplayFormat("m:ss:zzz");
	bar->setBad();

	// Layout
	layout->addWidget(lbeginning, 0, 0);
	layout->addWidget(beginning, 0, 1);
	layout->addWidget(bbeginning, 0, 2);

	layout->addWidget(lbar, 1, 0);
	layout->addWidget(bar, 1, 1);
	layout->addWidget(bbar, 1, 2);

	layout->addWidget(lbpm, 2, 0);
	layout->addWidget(bpm, 2, 1);

	layout->addWidget(lend, 3, 0);
	layout->addWidget(end, 3, 1);
	layout->addWidget(bend, 3, 2);

	layout->addWidget(sendButton, 4, 0);

	setLayout(layout);

	// Connections
	connect(bbeginning, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
	connect(bend, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
	connect(bbar, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));

	connect(bpm, SIGNAL(valueChanged(int)), this, SLOT(tempoChanged(int)));
	connect(bpm, SIGNAL(hasBeenClicked()), this, SLOT(tempoClicked()));

	connect(beginning, SIGNAL(timeChanged(QTime)), this, SLOT(beginningChanged(QTime)));
	connect(bar, SIGNAL(timeChanged(QTime)), this, SLOT(barChanged(QTime)));
	connect(end, SIGNAL(timeChanged(QTime)), this, SLOT(endChanged(QTime)));

	connect(sendButton, SIGNAL(clicked()), this, SLOT(sendData()));
}

/**
 * @brief AudioSync::~AudioSync
 *
 * Destructeur.
 */
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
	bpm->setEnabled(active);

	if(active)
	{
		endChanged(end->time());
		beginningChanged(beginning->time());
		barChanged(bar->time());
		tempoChanged(bpm->value());
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
 *
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
	updateTempo();

	emit sendTimer(TIMER_BEGINNING, t);
}
/**
 * @brief AudioSync::updateTempo
 *
 * Met à jour l'information de tempo en fonction de la différence de temps entre bar et beginning
 */
void AudioSync::updateTempo()
{
	int tps = TimeToMsec(bar->time()) - TimeToMsec(beginning->time());
	if(tps > 0)
	{
		int tempo = m_timeSignature * 60000 / tps ;

		if(tempo >= 10 && tempo <= 250 && tempo != bpm->value())
		{
			bpm->setValue(tempo);
			bpm->setGood();
		}
		else if((tempo < 10 || tempo > 250) && tempo != bpm->value())
		{
			bpm->setBad();
		}
	}
}

/**
 * @brief AudioSync::barChanged
 * @param t Temps de la mesure
 *
 * Utilisé principalement pour modifier la waveform en temps réel
 */
void AudioSync::barChanged(QTime t)
{
	checkTimes();
	updateTempo();

	emit sendTimer(TIMER_BAR, t);
}

/**
 * @brief AudioSync::endChanged
 * @param t Temps de la fin
 *
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
	long endTime = TimeToMsec(end->time());
	long beginningTime = TimeToMsec(beginning->time());
	long barTime = TimeToMsec(bar->time());

	// Condition sur end
	if(endTime - beginningTime  > 0)
	{
		end->setGood();
	}
	else
	{
		end->setBad();
	}

	//Condition sur bar
	if(endTime - barTime > 0 &&
	   barTime - beginningTime > 0)
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
	// on ne change bar que si tempo a été modifié manuellement
	m_tempBarTime = MsecToTime(m_timeSignature * 60000 / tempo + TimeToMsec(beginning->time()));
}

/**
 * @brief AudioSync::emitSignalTimer
 *
 * Utilisé pour mettre les temps en fonction de la position du player
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
		QMessageBox::information(this, tr("Your job is not done yet"), tr("The three timers have not been set yet."));
}

/**
 * @brief AudioSync::recvTimer
 * @param type Type de timer reçu (début, mesure, fin)
 * @param t Temps
 *
 * Utilisé pour recevoir un timer (lorsqu'on bouge les WaveformTimeSlider)
 */
void AudioSync::recvTimer(int type, QTime t)
{
	if(type == TIMER_BEGINNING)
	{
		beginning->setTime(t);
	}
	else if(type == TIMER_BAR)
	{
		bar->setTime(t);
	}
	else if(type == TIMER_END)
	{
		end->setTime(t);
	}
}

/**
 * @brief AudioSync::tempoClicked
 *
 * Slot appelé lorsqu'on change manuellement la valeur du tempo
 */
void AudioSync::tempoClicked()
{
	bar->setTime(m_tempBarTime);
	bpm->setGood();
}

/**
 * @brief AudioSync::setTimeSignature
 * @param t Signature rythmique. ex: 1 pour 1/4, 2 pour 2/4, 4 pour 4/4...
 *
 * Utilisé pour appliquer la signature rythmique aux informations de temps.
 */
void AudioSync::setTimeSignature(int t)
{
	int previous = m_timeSignature;
	m_timeSignature = t;
	setBarTimer(MsecToTime(TimeToMsec(bar->time()) * m_timeSignature / previous));
	updateTempo();
}

int AudioSync::getBar(){
	QTime t = bar->time();
	int ms = t.hour()*60*60*100;//conversion d'une heure en ms
	ms += t.minute()*60*100;
	ms += t.second()*100;
	ms += t.msec();
	return ms;
}

int AudioSync::getBeginning(){
	QTime t = beginning->time();
	int ms = t.hour()*60*60*100;//conversion d'une heure en ms
	ms += t.minute()*60*100;
	ms += t.second()*100;
	ms += t.msec();
	return ms;
}

int AudioSync::getEnd(){
	QTime t = end->time();
	int ms = t.hour()*60*60*100;//conversion d'une heure en ms
	ms += t.minute()*60*100;
	ms += t.second()*100;
	ms += t.msec();
	return ms;
}
