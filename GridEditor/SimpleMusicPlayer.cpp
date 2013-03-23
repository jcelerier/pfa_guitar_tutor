#include "SimpleMusicPlayer.h"
#include "AudioWindow.h"
#include <Util.hpp>
#include <QtWidgets/QMessageBox>
#include <QDebug>


//trucs pour le zoom
#define MOVE_FACTOR 3
#define ZOOM_FACTOR 20

//nécessaire pour emplir l'espace, don't ask me why.. Sans doute à cause des 9 pixels de marge de chaque côté
#define WIDTH_ADJUSTMENT 18
/**
 * @brief SimpleMusicPlayer::SimpleMusicPlayer
 *
 * Construit l'interface du lecteur audio.
 */
SimpleMusicPlayer::SimpleMusicPlayer(QWidget* parent) : QWidget(parent)
{
    this->parent = parent;
    layout = new QGridLayout();
    playButton = new QToolButton();
    // A FAIRE : playBarButton = new QToolButton();
    stopButton = new QToolButton();
    slideBar = new QSlider(Qt::Horizontal);
    timerLabel = new QLabel("");
    timer = new QTimer();
    playTimer = new QTimer();


    player = new MusicPlayer();

    waveform = new Waveform(this, ((AudioWindow*) parent)->width()  - WIDTH_ADJUSTMENT, 300);
    waveformTimeBar = new WaveformTimeBar(QTime(0, 0), this);
    ((AudioWindow*) parent)->setWaveformData(waveform, waveformTimeBar);


    playButton->setIcon(QIcon(":/icons/play.png"));
    //playBarButton->setIcon(QIcon(":/icons/play.png"));
    stopButton->setIcon(QIcon(":/icons/stop.png"));

    layout->addWidget(slideBar, 0, 0, 1, 7);
    layout->addWidget(playButton, 1, 0);
    //layout->addWidget(playBarButton, 1, 1);
    layout->addWidget(stopButton, 1, 2);
    layout->addWidget(timerLabel, 1, 6);

    setLayout(layout);

    waveform->setWidth(parent->width() - WIDTH_ADJUSTMENT);
    waveform->initImage();
    waveform->update();

    connect(playButton, SIGNAL(released()), this, SLOT(pause()));
    //connect(playBarButton, SIGNAL(clicked()), this, SLOT(playBar()));
    connect(stopButton, SIGNAL(released()), this, SLOT(stop()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSlideBar()));
    connect(playTimer, SIGNAL(timeout()), this, SLOT(sendTimeData()));

    connect(this, SIGNAL(sigTimeData(QTime)), waveform, SLOT(setPlayerTimer(QTime)));
    connect(this, SIGNAL(sigTimeData(QTime)), waveformTimeBar, SLOT(setPlayerTimer(QTime)));


    connect(slideBar, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));

    currentPosition = 0;
    songLength = 0;
    refreshTimerLabel();
}

/**
 * @brief SimpleMusicPlayer::~SimpleMusicPlayer
 */
SimpleMusicPlayer::~SimpleMusicPlayer()
{
    delete slideBar;
    delete layout;
    delete playButton;
    // delete playBarButton;
    delete playTimer;
    delete player;
    delete waveform;
    delete waveformTimeBar;
    delete stopButton;
    delete timerLabel;
    delete timer;
}



/**
 * @brief SimpleMusicPlayer::getSong
 * @return Le chemin vers le fichier audio courant.
 *
 * Retourne le chemin vers le fichier audio courant.
 */
QString SimpleMusicPlayer::getSong()
{
    return player->getSong();
}


/**
 * @brief SimpleMusicPlayer::setAudioFile
 * @param file Chemin vers le fichier audio.
 * @return Vrai si et seulement si le fichier audio a été ouvert correctement.
 *
 * Ouvre un nouveau fichier audio pour la lecture.
 */
bool SimpleMusicPlayer::setAudioFile(QString file)
{
    if(file == ""){
        return false;
    }

    if(!player->setSong(file)) {
        QMessageBox::information(this, tr("So sorry..."), QString(tr("Impossible to open the file ") + file));
        return false;
    }
    songLength = player->getTotalLength();
    refreshTimerLabel();
    slideBar->setRange(0, songLength);

    waveBegin = 0;
    waveEnd = player->getTotalLengthInSamples();

    waveform->setWidth(parent->width() - WIDTH_ADJUSTMENT);
    player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
    waveform->activate();
    waveUpdate();


    player->play();
    player->pause(true);
    timer->start(REFRESH_DELAY);

    return true;
}

/**
 * @brief SimpleMusicPlayer::resizeEvent
 * @param event QResizeEvent reçu de l'évènement
 *
 * Actualise le graphe en cas de redimensionnement de la fenêtre.
 */
void SimpleMusicPlayer::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    if(player->getState())
    {
        waveform->setWidth(parent->width() - WIDTH_ADJUSTMENT);

        player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());

    }
    else
    {
        waveform->setWidth(parent->width() - WIDTH_ADJUSTMENT);
    }
    waveUpdate();
}


/**
 * @brief SimpleMusicPlayer::getCurrentPosition
 * @return Donne la position actuelle dans le morceau.
 */
QTime SimpleMusicPlayer::getCurrentPosition()
{
    return MsecToTime(player->getPosition());
}

/**
 * @brief SimpleMusicPlayer::play
 *
 * Lecture du fichier audio choisi.
 */
void SimpleMusicPlayer::play()
{
    if(player->getState())
    {
        player->pause(false);
        playTimer->start(PLAYTIMER_DELAY);
    }
    else
        emit browseAudioFile();
}

/**
 * @brief SimpleMusicPlayer::sendTimeData
 *
 * Envoie le temps de lecture actuel
 */
void SimpleMusicPlayer::sendTimeData()
{
    emit sigTimeData(getCurrentPosition());
}

/**
 * @brief SimpleMusicPlayer::pause
 *
 * Mise en pause de la lecture.
 */
void SimpleMusicPlayer::pause()
{
    if(player->getState())
    {

        if(player->isPaused())
        {
            player->pause(false);
            playTimer->start(PLAYTIMER_DELAY);
        }
        else
        {
            player->pause(true);
            playTimer->stop();
        }


        if(player->isPaused())
            playButton->setIcon(QIcon(":/icons/pause.png"));
        else
            playButton->setIcon(QIcon(":/icons/play.png"));
    }
}

/**
 * @brief SimpleMusicPlayer::stop
 *
 * Arrêt de la lecture.
 */
void SimpleMusicPlayer::stop()
{
    if(player->getState()) {
        player->stop();
        playTimer->stop();
        currentPosition = 0;
        slideBar->setSliderPosition(0);
        refreshTimerLabel();
        playButton->setIcon(QIcon(":/icons/play.png"));
    }
}

/**
 * @brief SimpleMusicPlayer::refreshTimerLabel
 *
 * Rafraîchissement de l'affichage du chronomètre du lecteur.
 */
void SimpleMusicPlayer::refreshTimerLabel()
{
    QString tmp1, tmp2;
    if(currentPosition > 0) {
        int min = (currentPosition/1000)/60;
        int sec = (currentPosition/1000)%60;
        if(sec<10)
            timerLabel->setText(QString(tmp1.setNum(min) + ":0" + tmp2.setNum(sec) + " / "));
        else
            timerLabel->setText(QString(tmp1.setNum(min) + ":" + tmp2.setNum(sec) + " / "));
    }
    else {
        if(player->getState())
            timerLabel->setText("0:00 / ");
        else
            timerLabel->setText(".. / ");
    }
    if(songLength > 0) {
        int min = (songLength/1000)/60;
        int sec = (songLength/1000)%60;
        if(sec<10)
            timerLabel->setText(QString(timerLabel->text() + tmp1.setNum(min) + ":0" + tmp2.setNum(sec)));
        else
            timerLabel->setText(QString(timerLabel->text() + tmp1.setNum(min) + ":" + tmp2.setNum(sec)));
    }
    else
        timerLabel->setText(QString(timerLabel->text() + ".."));
}

/**
 * @brief SimpleMusicPlayer::updateSlideBar
 *
 * Rafraîchissement de la barre de défilement.
 */
void SimpleMusicPlayer::updateSlideBar()
{
    currentPosition = player->getPosition();
    slideBar->setSliderPosition(currentPosition);
    refreshTimerLabel();
    timer->start(REFRESH_DELAY);
}

/**
 * @brief SimpleMusicPlayer::changePosition
 * @param position Nouvelle position exprimée en millisecondes
 *
 * Demande à aller à un temps précis dans le morceau courant.
 */
void SimpleMusicPlayer::changePosition(int position)
{
    player->changePosition(position);
    currentPosition = player->getPosition();
}


/**
 * @brief SimpleMusicPlayer::zoomIn
 * @param clickPos position du pointeur
 *
 * Effectue un zoom avant sur la waveform.
 * Algorithme : on augmente les positions gauche et droite de zoomFactor%
 *			relativement à la position du clic
 */
void SimpleMusicPlayer::zoomIn(QPoint clickPos)
{
    float clickPercent = (float) clickPos.x() / (float) waveform->getWidth();
    float sample = clickPercent * (waveEnd - waveBegin) + waveBegin;
    const int zoomFactor = ZOOM_FACTOR;

    if(waveEnd - waveBegin > 10000)
    {
        waveBegin += (sample - (float) waveBegin) / (float) zoomFactor;
        waveEnd -= (waveEnd - sample) / zoomFactor;

        if(waveEnd < waveBegin + 10000 || waveEnd < 0)
            waveEnd = waveBegin + 10000;

        player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
    }
    else
    {
        waveEnd = waveBegin + 10000;
    }
    waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::zoomOut
 * @param clickPos Position du clic
 *
 * Effectue un zoom arrière.
 */
void SimpleMusicPlayer::zoomOut(QPoint clickPos)
{
    float clickPercent = (float) clickPos.x() / (float) waveform->getWidth();
    float sample = clickPercent * (waveEnd - waveBegin) + waveBegin;
    const int zoomFactor = ZOOM_FACTOR;

    waveBegin = qMax(0,
                         qMin(waveBegin - (int) ((sample - (float) waveBegin) / (float) zoomFactor),
                                  (int) player->getTotalLengthInSamples() - 10000));
    waveEnd = qMax(waveBegin + 10000,
                       qMin((signed) player->getTotalLengthInSamples(),
                                waveEnd + qMax((int) ((waveEnd - sample) / zoomFactor),
                                                   0)));

    player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
    waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::moveLeft
 *
 * Effectue un déplacement à gauche.
 */
void SimpleMusicPlayer::moveLeft()
{
    int mvt = (waveEnd - waveBegin) * MOVE_FACTOR / waveform->getWidth() ;

    if(waveBegin > 0)
    {
        if(waveBegin - mvt >= 0) // cas valide
        {
            waveBegin -= mvt;
            waveEnd -= mvt;
        }
        else if(waveBegin - mvt < 0)// cas ou on est au bord
        {
            waveBegin = 0;
        }
    }

    player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
    waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::moveRight
 *
 * Effectue un déplacement à droite.
 */
void SimpleMusicPlayer::moveRight()
{
    int lgr = player->getTotalLengthInSamples();

    int mvt = (waveEnd - waveBegin) * MOVE_FACTOR / waveform->getWidth() ;

    if(waveEnd < lgr)
    {
        if (waveEnd + mvt <= lgr)
        {
            waveBegin += mvt;
            waveEnd += mvt;
        }
        else
        {
            waveEnd = lgr;
        }
    }

    player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
    waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::getWaveBegin
 * @return Sample gauche de la waveform
 */
int SimpleMusicPlayer::getWaveBegin()
{
    return waveBegin;
}

/**
 * @brief SimpleMusicPlayer::getWaveEnd
 * @return Sample droite de la waveform
 */
int SimpleMusicPlayer::getWaveEnd()
{
    return waveEnd;
}


/**
 * @brief SimpleMusicPlayer::waveFullZoom
 *
 * Zoom de manière à ce qu'on voie l'intégralité du fichier
 */
void SimpleMusicPlayer::waveFullZoom()
{
    waveBegin = 0;
    waveEnd = player->getTotalLengthInSamples();
    player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
    waveUpdate();
}

/**
 * @brief SimpleMusicPlayer::waveSongZoom
 *
 * Zoom de manière à ce qu'on voie le morceau tel qu'il a été défini dans AudioSync
 */

void SimpleMusicPlayer::waveSongZoom()
{
    if(waveform->getSampleBegin() + 10000 < waveform->getSampleEnd())
    {
        waveBegin = waveform->getSampleBegin();
        waveEnd = waveform->getSampleEnd();
        player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
        waveUpdate();
    }
}
/**
 * @brief SimpleMusicPlayer::waveBarZoom
 *
 * Définit le zoom de manière à ce qu'on voie la première mesure
 */
void SimpleMusicPlayer::waveBarZoom()
{
    if(waveform->getSampleBegin() + 10000 < waveform->getSampleBar())
    {
        waveBegin = waveform->getSampleBegin();
        waveEnd = waveform->getSampleBar();
        player->getSpectrum(waveBegin, waveEnd, waveform->getSpectrum(), waveform->getWidth());
        waveUpdate();
    }
}

/**
 * @brief SimpleMusicPlayer::waveUpdate
 *
 * Mise à jour de la forme d'onde.
 */
void SimpleMusicPlayer::waveUpdate()
{
    waveform->update();
    waveformTimeBar->update();
}
