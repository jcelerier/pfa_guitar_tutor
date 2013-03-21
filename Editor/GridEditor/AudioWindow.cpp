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
	this->parent = parent;
	player = new SimpleMusicPlayer(this); // met en place la waveform via setWaveformData
	label = new QLabel(tr("Audio file")); //fuite mémoire
	audioFile = new QLineEdit();
	audioFile->setReadOnly(true);
	audioFile->setEnabled(false);
	browseButton = new QPushButton(tr("Browse"));
	audioSync = new AudioSync(this);

	layout = new QGridLayout();

	zoomButtons = new ZoomButtons(this);

	layout->addWidget(audioSync, 1, 0, 3, 1);
	layout->addWidget(label, 1, 1);
	layout->addWidget(audioFile, 1, 2);
	layout->addWidget(browseButton, 1, 3);
	layout->addWidget(player, 2, 1, 3, 3);
	layout->addWidget(waveformTimeBar, 4, 0, 1, 5);

	layout->addWidget(waveform, 5, 0, 3, 5);
	layout->addWidget(zoomButtons, 12, 0, 1, 1);

	connect(browseButton, SIGNAL(released()), this, SLOT(browseAudioFile()));
	connect(player, SIGNAL(browseAudioFile()), this, SLOT(browseAudioFile()));
	connect(player, SIGNAL(audioFileDeleted()), this, SLOT(resetAudioFile()));

	connect(player, SIGNAL(sigTimeData(QTime)), parent, SIGNAL(sigTimeData(QTime)));
	connect(audioSync, SIGNAL(refreshTimer(int)), this, SLOT(refreshTimerAudioSync(int)));
	connect(audioSync, SIGNAL(sendTimers(QTime, QTime, QTime)), parent, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)));

	connect(audioSync, SIGNAL(sendTimer(int,QTime)), waveform, SLOT(setTimer(int, QTime)));
	connect(audioSync, SIGNAL(sendTimer(int,QTime)), waveformTimeBar, SLOT(setTimer(int, QTime)));

	connect(this, SIGNAL(timeSignatureChanged(int)), audioSync, SLOT(setTimeSignature(int)));

	connect(this, SIGNAL(waveFullZoom()), player, SLOT(waveFullZoom()));
	connect(this, SIGNAL(waveBarZoom()), player, SLOT(waveBarZoom()));
	connect(this, SIGNAL(waveSongZoom()), player, SLOT(waveSongZoom()));

	connect(waveformTimeBar, SIGNAL(timeChanged(int,QTime)), audioSync, SLOT(recvTimer(int, QTime)));

	this->setLayout(layout);
}

/**
 * @brief AudioWindow::~AudioWindow
 *
 * Destructeur.
 */
AudioWindow::~AudioWindow()
{
	delete player;
	delete audioFile;
	delete browseButton;
	delete audioSync;
	delete zoomButtons;
	delete layout;
	delete label;
}

/**
 * @brief AudioWindow::setWaveform
 * @param waveform Une forme d'onde
 */
void AudioWindow::setWaveformData(Waveform* waveform, WaveformTimeBar* timeBar)
{
	this->waveform = waveform;
	this->waveformTimeBar = timeBar;
}

/**
 * @brief AudioWindow::setAudioFile
 *
 * Modifie l'affichage du fichier audio associé.
 */
void AudioWindow::setAudioFile()
{
	audioFile->setEnabled(true);
	audioFile->setText(player->getSong());
	audioSync->activeButtons(true);
	waveformTimeBar->activate();
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
	player->setAudioFile(file);
}

/**
 * @brief AudioWindow::resetAudioFile
 *
 * Réinitialise l'affichage du fichier audio associé.
 */
void AudioWindow::resetAudioFile()
{
	audioFile->setText("");
	audioFile->setEnabled(false);
	audioSync->activeButtons(false);
	waveformTimeBar->deactivate();
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
        audioSync->setBeginningTimer(player->getCurrentPosition());
		break;
	case TIMER_END:
		audioSync->setEndTimer(player->getCurrentPosition());
		break;
	case TIMER_BAR:
		audioSync->setBarTimer(player->getCurrentPosition());
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
		if(player->setAudioFile(tmp))
			setAudioFile();
	}
}

/**
 * @brief AudioWindow::getFilename
 * @return Nom du fichier audio
 */
QString AudioWindow::getFilename()
{
	return player->getSong();
}


/**
 * @brief AudioWindow::playFrom
 * @param t Temps à partir du quel on joue en millisecondes
 *
 * Joue le fichier audio à partir du temps t.
 */
void AudioWindow::playFrom(int t)
{
	player->play();
	player->changePosition( t);
}

/**
 * Entrée : Nombre de millisecondes correspondant à la position de la bar dans l'éditeur.
 *
 * Modifie la valeur de bar dans audioSync.
 */
void AudioWindow::setBar(const int bar){
    QTime t(0, 0);
    t = t.addMSecs(bar);
    return audioSync->setBarTimer(t);
}

/**
 * Entrée : Nombre de millisecondes correspondant à la position du beginning dans l'éditeur.
 *
 * Modifie la valeur de beginning dans audioSync
 */
void AudioWindow::setBeginning(const int begin){
    QTime t(0, 0);
    t = t.addMSecs(begin);
    return audioSync->setBeginningTimer(t);
}

/**
 * Entrée : Nombre de millisecondes correspondant à la position de end dans l'éditeur.
 *
 * Modifie la valeur de end dans audioSync
 */
void AudioWindow::setEnd(const int end){
    QTime t(0, 0);
    t = t.addMSecs(end);
    return audioSync->setEndTimer(t);
}

/**
 * Retourne la valeur de la bar entrée par l'utilisateur
 */
int AudioWindow::getBar(){
	return audioSync->getBar();
}

/**
 * Retourne la valeur de la bar entrée par l'utilisateur
 */
int AudioWindow::getBeginning(){
	return audioSync->getBeginning();
}

/**
 * Retourne la valeur de la bar entrée par l'utilisateur
 */
int AudioWindow::getEnd(){
	return audioSync->getEnd();
}
