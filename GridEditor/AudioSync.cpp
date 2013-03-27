#include "AudioSync.h"
#include <Util.hpp>
#include <QtWidgets/QMessageBox>


/**
 * @brief AudioSync::AudioSync
 * Constructeur des boutons de synchronisation audio
 */
AudioSync::AudioSync(QWidget* parent) : QWidget(parent), m_timeSignature(4)
{
	m_layout = new QGridLayout();
	m_beginning = new TimeEdit();
	m_end = new TimeEdit();
	m_bar = new TimeEdit();
	m_bpm = new TempoEdit();

	m_lbeginning = new QLabel(tr("Beginning"));
	m_lend = new QLabel(tr("End"));
	m_lbar = new QLabel(tr("Bar"));
	m_lbpm = new QLabel(tr("BPM"));

	m_bbeginning = new QToolButton();
	m_bend = new QToolButton();
	m_bbar = new QToolButton();

	m_sendButton = new QPushButton(tr("Compute"));

	// Propriétés du GUI
	activeButtons(false);

	m_bbeginning->setIcon(QIcon(":/icons/timer.png"));
	m_bend->setIcon(QIcon(":/icons/timer.png"));
	m_bbar->setIcon(QIcon(":/icons/timer.png"));

	m_beginning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	m_bpm->setMinimum(10);
	m_bpm->setMaximum(250);
	m_bpm->setBad();

	m_beginning->setDisplayFormat("m:ss:zzz");

	m_end->setDisplayFormat("m:ss:zzz");
	m_end->setBad();

	m_bar->setDisplayFormat("m:ss:zzz");
	m_bar->setBad();

	// Layout
	m_layout->addWidget(m_lbeginning, 0, 0);
	m_layout->addWidget(m_beginning, 0, 1);
	m_layout->addWidget(m_bbeginning, 0, 2);

	m_layout->addWidget(m_lbar, 1, 0);
	m_layout->addWidget(m_bar, 1, 1);
	m_layout->addWidget(m_bbar, 1, 2);

	m_layout->addWidget(m_lbpm, 2, 0);
	m_layout->addWidget(m_bpm, 2, 1);

	m_layout->addWidget(m_lend, 3, 0);
	m_layout->addWidget(m_end, 3, 1);
	m_layout->addWidget(m_bend, 3, 2);

	m_layout->addWidget(m_sendButton, 4, 0);

	setLayout(m_layout);

	// Connections
	connect(m_bbeginning, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
	connect(m_bend, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));
	connect(m_bbar, SIGNAL(pressed()), this, SLOT(emitSignalTimer()));

	connect(m_bpm, SIGNAL(valueChanged(int)), this, SLOT(tempoChanged(int)));
	connect(m_bpm, SIGNAL(hasBeenClicked()), this, SLOT(tempoClicked()));

	connect(m_beginning, SIGNAL(timeChanged(QTime)), this, SLOT(beginningChanged(QTime)));
	connect(m_bar, SIGNAL(timeChanged(QTime)), this, SLOT(barChanged(QTime)));
	connect(m_end, SIGNAL(timeChanged(QTime)), this, SLOT(endChanged(QTime)));

	connect(m_sendButton, SIGNAL(clicked()), this, SLOT(sendData()));
}

/**
 * @brief AudioSync::~AudioSync
 *
 * Destructeur.
 */
AudioSync::~AudioSync()
{
	delete m_bpm;
	delete m_beginning;
	delete m_end;
	delete m_bar;

	delete m_layout;

	delete m_bbeginning;
	delete m_bend;
	delete m_bbar;

	delete m_lbpm;
	delete m_lend;
	delete m_lbeginning;
	delete m_lbar;
}

/**
 * @brief AudioSync::activeButtons
 * @param active Vrai si les boutons doivent être activés, faux sinon
 *
 * Active l'interface AudioSync (rend les éléments clickables)
 */
void AudioSync::activeButtons(bool active)
{
	m_beginning->setEnabled(active);
	m_end->setEnabled(active);
	m_bar->setEnabled(active);
	m_bpm->setEnabled(active);

	if(active)
	{
		endChanged(m_end->time());
		beginningChanged(m_beginning->time());
		barChanged(m_bar->time());
		tempoChanged(m_bpm->value());
	}
}

/**
 * @brief AudioSync::setBegginingTimer
 * @param t Temps du début
 *
 * Appelé principalement lors de la pression du petit bouton à la droite du TimeEdit
 */
void AudioSync::setBeginningTimer(const QTime t)
{
	m_beginning->setTime(t);
}

/**
 * @brief AudioSync::setEndTimer
 * @param t Temps de la fin

 * Appelé principalement lors de la pression du petit bouton à la droite du TimeEdit
 */
void AudioSync::setEndTimer(const QTime t)
{
	m_end->setTime(t);
}

/**
 * @brief AudioSync::setBarTimer
 * @param t Temps de la mesure
 *
 * Appelé principalement lors de la pression du petit bouton à la droite du TimeEdit
 */
void AudioSync::setBarTimer(const QTime t)
{
	m_bar->setTime(t);
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
	int tps = TimeToMsec(m_bar->time()) - TimeToMsec(m_beginning->time());
	if(tps > 0)
	{
		int tempo = m_timeSignature * 60000 / tps ;

		if(tempo >= 10 && tempo <= 250 && tempo != m_bpm->value())
		{
			m_bpm->setValue(tempo);
			m_bpm->setGood();
		}
		else if((tempo < 10 || tempo > 250) && tempo != m_bpm->value())
		{
			m_bpm->setBad();
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
	long endTime = TimeToMsec(m_end->time());
	long beginningTime = TimeToMsec(m_beginning->time());
	long barTime = TimeToMsec(m_bar->time());

	// Condition sur end
	if(endTime - beginningTime  > 0)
	{
		m_end->setGood();
	}
	else
	{
		m_end->setBad();
	}

	//Condition sur bar
	if(endTime - barTime > 0 &&
	   barTime - beginningTime > 0)
	{
		m_bar->setGood();
	}
	else
	{
		m_bar->setBad();
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
	m_tempBarTime = MsecToTime(m_timeSignature * 60000 / tempo + TimeToMsec(m_beginning->time()));
}

/**
 * @brief AudioSync::emitSignalTimer
 *
 * Utilisé pour mettre les temps en fonction de la position du player
 */
void AudioSync::emitSignalTimer()
{
	if(m_bbeginning->isDown())
		emit refreshTimer(TIMER_BEGINNING);
	else if(m_bend->isDown())
		emit refreshTimer(TIMER_END);
	else if(m_bbar->isDown())
		emit refreshTimer(TIMER_BAR);
}

/**
 * @brief AudioSync::sendData
 *
 * Envoie les données à GridEditor
 */
void AudioSync::sendData()
{
	if(m_beginning->time().isValid() && m_bar->time().isValid() && m_end->time().isValid())
	{
		emit sendTimers(m_beginning->time(), m_bar->time(), m_end->time());
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
		m_beginning->setTime(t);
	}
	else if(type == TIMER_BAR)
	{
		m_bar->setTime(t);
	}
	else if(type == TIMER_END)
	{
		m_end->setTime(t);
	}
}

/**
 * @brief AudioSync::tempoClicked
 *
 * Slot appelé lorsqu'on change manuellement la valeur du tempo
 */
void AudioSync::tempoClicked()
{
	m_bar->setTime(m_tempBarTime);
	m_bpm->setGood();
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
	setBarTimer(MsecToTime(TimeToMsec(m_bar->time()) * m_timeSignature / previous));
	updateTempo();
}

/**
 * @brief AudioSync::getBar
 * @return Le temps de fin de la première mesure en ms.
 *
 * Retourne le temps de fin de la première mesure en ms.
 */
int AudioSync::getBar(){
	return TimeToMsec(m_bar->time());
}

/**
 * @brief AudioSync::getBeginning
 * @return Le temps de début de la première mesure en ms.
 *
 * Retourne le temps de début de la première mesure en ms.
 */
int AudioSync::getBeginning(){
	return TimeToMsec(m_beginning->time());
}

/**
 * @brief AudioSync::getEnd
 * @return Le temps de fin du morceau en ms.
 *
 * Retourne le temps de fin du morceau en ms.
 */
int AudioSync::getEnd(){
	return TimeToMsec(m_end->time());
}
