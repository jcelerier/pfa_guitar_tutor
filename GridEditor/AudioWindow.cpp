#include "AudioWindow.h"
#include <QFileDialog>

/**
 * @brief AudioWindow::AudioWindow
 * @param parent Widget parent
 *
 * Constructeur.
 */
AudioWindow::AudioWindow(QWidget * parent)
{
	m_player = new SimpleMusicPlayer(this); // met en place la waveform via setWaveformData
	m_label = new QLabel(tr("Audio file")); //fuite mémoire
	m_audioFile = new QLineEdit();
	m_audioFile->setReadOnly(true);
	m_audioFile->setEnabled(false);
	m_browseButton = new QPushButton(tr("Browse"));
	m_audioSync = new AudioSync(this);

	m_layout = new QGridLayout();

	m_zoomButtons = new ZoomButtons(this);

	m_layout->addWidget(m_audioSync, 1, 0, 3, 1);
	m_layout->addWidget(m_label, 1, 1);
	m_layout->addWidget(m_audioFile, 1, 2);
	m_layout->addWidget(m_browseButton, 1, 3);
	m_layout->addWidget(m_player, 2, 1, 3, 3);
	m_layout->addWidget(m_waveformTimeBar, 4, 0, 1, 5);

	m_layout->addWidget(m_waveform, 5, 0, 3, 5);
	m_layout->addWidget(m_zoomButtons, 12, 0, 1, 1);

	connect(m_waveformTimeBar, SIGNAL(somethingChanged()), this, SIGNAL(somethingChanged()));

	connect(m_browseButton, SIGNAL(released()), this, SLOT(browseAudioFile()));
	connect(m_player, SIGNAL(browseAudioFile()), this, SLOT(browseAudioFile()));

	connect(m_player, SIGNAL(sigTimeData(QTime)), parent, SIGNAL(sigTimeData(QTime)));
	connect(m_audioSync, SIGNAL(refreshTimer(int)), this, SLOT(refreshTimerAudioSync(int)));
	connect(m_audioSync, SIGNAL(sendTimers(QTime, QTime, QTime)), parent, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)));

	connect(m_audioSync, SIGNAL(sendTimer(int,QTime)), m_waveform, SLOT(setTimer(int, QTime)));
	connect(m_audioSync, SIGNAL(sendTimer(int,QTime)), m_waveformTimeBar, SLOT(setTimer(int, QTime)));

	connect(this, SIGNAL(timeSignatureChanged(int)), m_audioSync, SLOT(setTimeSignature(int)));

	connect(this, SIGNAL(waveFullZoom()), m_player, SLOT(waveFullZoom()));
	connect(this, SIGNAL(waveBarZoom()), m_player, SLOT(waveBarZoom()));
	connect(this, SIGNAL(waveSongZoom()), m_player, SLOT(waveSongZoom()));

	connect(m_waveformTimeBar, SIGNAL(timeChanged(int,QTime)), m_audioSync, SLOT(recvTimer(int, QTime)));

	this->setLayout(m_layout);
}

/**
 * @brief AudioWindow::~AudioWindow
 *
 * Destructeur.
 */
AudioWindow::~AudioWindow()
{
	delete m_player;
	delete m_audioFile;
	delete m_browseButton;
	delete m_audioSync;
	delete m_zoomButtons;
	delete m_layout;
	delete m_label;
}

/**
 * @brief AudioWindow::setWaveform
 * @param waveform Une forme d'onde
 */
void AudioWindow::setWaveformData(Waveform* waveform, WaveformTimeBar* timeBar)
{
	this->m_waveform = waveform;
	this->m_waveformTimeBar = timeBar;
}

/**
 * @brief AudioWindow::setAudioFile
 *
 * Modifie l'affichage du fichier audio associé.
 */
void AudioWindow::setAudioFile()
{
	m_audioFile->setEnabled(true);
	m_audioFile->setText(m_player->getSong());
	m_audioSync->activeButtons(true);
	m_waveformTimeBar->activate();
}

/**
 * @brief AudioWindow::setAudioFileName
 * @param file Nom du fichier
 *
 * Permet de simplement définir le nom du fichier mp3
 * (au chargement d'un xml par exemple.)
 */
void AudioWindow::setAudioFileName(QString file)
{
	m_player->setAudioFile(file);
}

/**
 * @brief AudioWindow::refreshTimerAudioSync
 * @param i
 *
 * Change le timer i quand on clique sur le bouton i,
 * i étant début, fin, mesure
 */
void AudioWindow::refreshTimerAudioSync(int i)
{
	switch(i) {
	case TIMER_BEGINNING:
		m_audioSync->setBeginningTimer(m_player->getCurrentPosition());
		break;
	case TIMER_END:
		m_audioSync->setEndTimer(m_player->getCurrentPosition());
		break;
	case TIMER_BAR:
		m_audioSync->setBarTimer(m_player->getCurrentPosition());
		break;
	}
}

/**
 * @brief AudioWindow::browseAudioFile
 *
 * Demande de sélection d'un fichier audio sur le disque dur de l'utilisateur.
 */
void AudioWindow::browseAudioFile()
{
	QString tmp = QFileDialog::getOpenFileName(this, tr("Open a file"), QString(), tr("Music (*.mp3 *.ogg *.wma *.wav)"));
	if(tmp != "") {
		QDir d;
		tmp = d.relativeFilePath(tmp);
		if(m_player->setAudioFile(tmp))
			setAudioFile();
	}
}

/**
 * @brief AudioWindow::getFilename
 * @return Nom du fichier audio
 */
QString AudioWindow::getFilename()
{
	return m_player->getSong();
}


/**
 * @brief AudioWindow::playFrom
 * @param t Temps à partir du quel on joue en millisecondes
 *
 * Joue le fichier audio à partir du temps t.
 */
void AudioWindow::playFrom(int t)
{
	m_player->play();
	m_player->changePosition(t);
}

/**
 * Entrée : Nombre de millisecondes correspondant à la position de la bar dans l'éditeur.
 *
 * Modifie la valeur de bar dans audioSync.
 */
void AudioWindow::setBar(const int bar){
	QTime t(0, 0);
	t = t.addMSecs(bar);
	return m_audioSync->setBarTimer(t);
}

/**
 * Entrée : Nombre de millisecondes correspondant à la position du beginning dans l'éditeur.
 *
 * Modifie la valeur de beginning dans audioSync
 */
void AudioWindow::setBeginning(const int begin){
	QTime t(0, 0);
	t = t.addMSecs(begin);
	return m_audioSync->setBeginningTimer(t);
}

/**
 * Entrée : Nombre de millisecondes correspondant à la position de end dans l'éditeur.
 *
 * Modifie la valeur de end dans audioSync
 */
void AudioWindow::setEnd(const int end){
	QTime t(0, 0);
	t = t.addMSecs(end);
	return m_audioSync->setEndTimer(t);
}

/**
 * Retourne la valeur de la bar entrée par l'utilisateur
 */
int AudioWindow::getBar(){
	return m_audioSync->getBar();
}

/**
 * Retourne la valeur de la bar entrée par l'utilisateur
 */
int AudioWindow::getBeginning(){
	return m_audioSync->getBeginning();
}

/**
 * Retourne la valeur de la bar entrée par l'utilisateur
 */
int AudioWindow::getEnd(){
	return m_audioSync->getEnd();
}
