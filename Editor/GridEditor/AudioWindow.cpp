#include "AudioWindow.h"
#include <QFileDialog>

AudioWindow::AudioWindow(QWidget * parent)
{
	this->parent = parent;
	player = new SimpleMusicPlayer(this);
    label = new QLabel(tr("Audio file")); //fuite mémoire
	audioFile = new QLineEdit();
	audioFile->setReadOnly(true);
	audioFile->setEnabled(false);
	browseButton = new QPushButton(tr("Browse"));
	audioSync = new AudioSync();

	layout = new QGridLayout();

	ZoomButtons* zoomButtons = new ZoomButtons(this);

	layout->addWidget(audioSync, 1, 0, 3, 1);
	layout->addWidget(label, 1, 1);
	layout->addWidget(audioFile, 1, 2);
	layout->addWidget(browseButton, 1, 3);
	layout->addWidget(player, 2, 1, 3, 3);
	layout->addWidget(waveform, 5, 0, 3, 5);
	layout->addWidget(zoomButtons, 12, 0, 1, 1);

	connect(browseButton, SIGNAL(released()), this, SLOT(browseAudioFile()));
	connect(player, SIGNAL(browseAudioFile()), this, SLOT(browseAudioFile()));
	connect(player, SIGNAL(audioFileDeleted()), this, SLOT(resetAudioFile()));
	connect(player, SIGNAL(sigTimeData(QTime)), parent, SIGNAL(sigTimeData(QTime)));

	connect(audioSync, SIGNAL(refreshTimer(int)), this, SLOT(refreshTimerAudioSync(int)));

	connect(audioSync, SIGNAL(sendTimers(QTime, QTime, QTime)), parent, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)));
	connect(audioSync, SIGNAL(sendTimers(QTime,QTime,QTime)), waveform, SLOT(setTimers(QTime, QTime, QTime)));

	connect(audioSync, SIGNAL(sendTimer(int,QTime)), waveform, SLOT(setTimer(int, QTime)));

	connect(this, SIGNAL(waveFullZoom()), player, SLOT(waveFullZoom()));
	connect(this, SIGNAL(waveBarZoom()), player, SLOT(waveBarZoom()));
	connect(this, SIGNAL(waveSongZoom()), player, SLOT(waveSongZoom()));

	this->setLayout(layout);
}

AudioWindow::~AudioWindow()
{
	delete player;
	delete audioFile;
	delete browseButton;
	delete audioSync;
	delete layout;
    delete label;
}

/**
 * @brief AudioWindow::setWaveform
 * @param waveform une forme d'onde
 */
void AudioWindow::setWaveform(Waveform* waveform)
{
	this->waveform = waveform;
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
}

void AudioWindow::refreshTimerAudioSync(int i)
{
	switch(i) {
	case TIMER_BEGINNING:
		audioSync->setBegginingTimer(player->getCurrentPosition());
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
