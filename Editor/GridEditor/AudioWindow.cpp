#include "AudioWindow.h"

AudioWindow::AudioWindow(QWidget * parent)
{

	player = new SimpleMusicPlayer();
	QLabel* label = new QLabel(tr("Audio file")); //fuite mémoire
	audioFile = new QLineEdit();
	audioFile->setReadOnly(true);
	audioFile->setEnabled(false);
	browseButton = new QPushButton(tr("Browse"));
	audioSync = new AudioSync();

	layout = new QGridLayout();

	layout->addWidget(audioSync, 1, 0, 3, 1);
	layout->addWidget(label, 1, 1);
	layout->addWidget(audioFile, 1, 2);
	layout->addWidget(browseButton, 1, 3);
	layout->addWidget(player, 2, 1, 3, 3);


	connect(browseButton, SIGNAL(released()), this, SLOT(browseAudioFile()));
	connect(player, SIGNAL(browseAudioFile()), this, SLOT(browseAudioFile()));
	connect(player, SIGNAL(audioFileDeleted()), this, SLOT(resetAudioFile()));
	connect(audioSync, SIGNAL(refreshTimer(int)), this, SLOT(refreshTimerAudioSync(int)));

	connect(audioSync, SIGNAL(sendTimer(QTime, QTime, QTime)), parent, SIGNAL(sendTimeToChordWidget(QTime, QTime, QTime)));
	this->setLayout(layout);


}

AudioWindow::~AudioWindow()
{
	delete player;
	delete audioFile;
	delete browseButton;
	delete audioSync;
	delete layout;
	delete title;
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
	case TIMER_BEGGINING:
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

QString AudioWindow::getSong()
{
	return player->getSong();
}
