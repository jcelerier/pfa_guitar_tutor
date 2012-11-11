#include "player.h"
#include "visualisationthread.h"
#include<unistd.h>

Player::Player()
{
    /*Initialise QWidget*/
    topWidget = new QWidget(this,NULL);
    botWidget = new QWidget(this,NULL);

    /*Initialise 3 main layouts*/
    QHBoxLayout *layoutButton = new QHBoxLayout();
    QHBoxLayout *layoutTop = new QHBoxLayout();
    QVBoxLayout *layoutCentral = new QVBoxLayout();

    /*Initialise buttons*/
    playButton=new QPushButton("PLAY");
    pauseButton=new QPushButton("PAUSE");
    openButton=new QPushButton("OPEN");
    visualisation=new QPushButton("VISUALISER");
    playButton->setFocusPolicy(Qt::NoFocus);
    pauseButton->setFocusPolicy(Qt::NoFocus);
    openButton->setFocusPolicy(Qt::NoFocus);
    visualisation->setFocusPolicy(Qt::NoFocus);

    layoutButton->addWidget(playButton);
    layoutButton->addWidget(pauseButton);
    layoutButton->addWidget(openButton);
    layoutButton->addWidget(visualisation);

    /*Initialise play informations*/
    slideBar=new QSlider(Qt::Horizontal);
    slideBar->setEnabled(false);
    slideBar->setFixedSize(400,50);
    compteurTemps = new QLabel(this,NULL);
    compteurTemps->setText("00:00");
    image = new QLabel(this, NULL);
    image->setPixmap(QPixmap("images/stop.png"));
    image->setFixedHeight(80);
    led = new QLabel(this,NULL);
    led->setPixmap(QPixmap("images/ledvert.png"));

    chordTable = new ChordTableWidget();

    layoutCentral->addWidget(chordTable);
    layoutCentral->setAlignment(chordTable,Qt::AlignCenter);
    layoutCentral->addWidget(compteurTemps);
    layoutCentral->setAlignment(compteurTemps,Qt::AlignCenter);
    layoutCentral->addWidget(led);
    layoutCentral->setAlignment(led,Qt::AlignCenter);
    layoutCentral->addWidget(slideBar);
    layoutCentral->setAlignment(slideBar,Qt::AlignCenter);
    layoutCentral->addWidget(image);
    layoutCentral->setAlignment(image,Qt::AlignCenter);
    layoutCentral->addLayout(layoutButton);

    infoMusique = new QLabel(this,NULL);
    infoMusique->setText("Lecture en cours:           ");

    layoutTop->addWidget(infoMusique);


    botWidget->setLayout(layoutCentral);
    botWidget->move(100,250);
    botWidget->setFixedWidth(600);
    topWidget->setLayout(layoutTop);
    topWidget->move(100,20);
    topWidget->setFixedWidth(600);

    /*Initialise system*/
    FMOD::System_Create(&system);
    system->init(1, FMOD_INIT_NORMAL, 0);

    /*Initialise channel*/
    channel = 0;
    initSon();

    /*Initialise timer*/
    timer = new QTimer();
    timer->setInterval(300);

    /*Initialise menu*/
    QMenuBar* menuBar = new QMenuBar(this);
    exportData = new QAction("&Exporter",this);
    exportData->setShortcut(tr("CTRL+E"));
    openAction = new QAction("&Ouvrir une musique",this);
    openAction->setShortcut(tr("CTRL+O"));

    QMenu* menu = menuBar->addMenu("&File");
    menu->addAction(exportData);
    menu->addAction(openAction);

    /*Other initialisation*/
    isInVisualisationMode = false;

}

void Player::initSon(){
    sonEnCours = false;
    stop = false;
    paused = false;
}


Player::~Player(){
    sound->release();
    system->close();
    system->release();
    timer->stop();
    delete timer;
    delete playButton;
    delete pauseButton;
    delete openButton;
    delete slideBar;
    delete compteurTemps;
    delete infoMusique;
}

QPushButton* Player::getPlayButton(){
    return playButton;
}

QPushButton* Player::getPauseButton(){
    return pauseButton;
}

QPushButton* Player::getOpenButton(){
    return openButton;
}

QSlider* Player::getSlider(){
    return slideBar;
}

QTimer* Player::getTimer(){
    return timer;
}

void Player::refreshTimer()
{
    bool isPlaying;
    channel->isPlaying(&isPlaying);
    if(!isPlaying){
        if(!paused){
            image->setPixmap(QPixmap("images/stop.png"));
            stop = true;
        }
        timer->stop();
    }
    else{
        unsigned int ms;
        channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
        sec = (int) (ms/1000);

        d_min = (int) (sec/60)/10;
        sec -= d_min*600;
        min = (int) (sec/60);
        sec -= min*60;
        d_sec = (int) (sec/10);
        sec -= d_sec*10;
        sec = sec;
        /*
        if(sec == 10)
        {
            d_sec++;
        sec = 0;
        }


        if(d_sec == 6)
        {
            min++;
        sec = 0;
        d_sec = 0;
        }

        if(min == 10)
        {
            d_min++;
            min = 0;
        sec = 0;
        d_sec = 0;
        }*/
    }
        // Conversion du double sec en QString
        tempsEcouleSec = QString::number(sec);
        tempsEcouleDSec = QString::number(d_sec);
        tempsEcouleMin = QString::number(min);
        tempsEcouleDMin = QString::number(d_min);

        compteurTemps->setText(tempsEcouleDMin + tempsEcouleMin + ":" + tempsEcouleDSec + tempsEcouleSec + "/" + musicLengthDMin + musicLengthMin + ":" + musicLengthDSec + musicLengthSec);
        unsigned int pos;
        channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
        slideBar->setValue(pos);
}

void Player::open()
{

    nom_fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "WAV (*.wav)"); // Ouverture du fichier
    QFileInfo fi(nom_fichier);
    name = fi.fileName();

    std::cout << "filename : "<< name.toStdString() << std::endl;
    if(!nom_fichier.isEmpty())
    {
        // On vérifie qu'un son n'est pas en cours de lecture pour ne pas lire deux fichiers audio à la fois
        if(sonEnCours == true) // Si un son est en cours de lecture...
        {
            std::cout << "aucun son n'est en cours de lecture" << std::endl;
            sound->release();
        }
        /*Reset the tempo vector*/
        while(!tempo.empty())
            tempo.erase(tempo.begin());

        std::cout << nom_fichier.toStdString() << std::endl;
        char *son = new char [nom_fichier.toStdString().size() + 1];
        strcpy (son,nom_fichier.toStdString().c_str());
        std::cout << "son : "<< son << std::endl;
        system->createSound(son, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &sound);

        std::cout << "création du son" << std::endl;
        /*
        system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
        paused = false;
        image->setPixmap(QPixmap("images/play.png"));

        std::cout << "lecture du son" << std::endl;*/
        sonEnCours = true;
        stop = true;

        sec = 0;
        min = 0;
        d_sec = 0;
        d_min = 0;

        infoMusique->setText("Lecture en cours: " + name);

        slideBar->setEnabled(true);
    }
    unsigned int musicLength;
    sound->getLength(&musicLength,FMOD_TIMEUNIT_MS);
    slideBar->setRange(0,musicLength);

    double seconde = (double) (musicLength /= 1000);
    double dMin = (int) (seconde/60)/10;
    seconde -= dMin*600;
    double min = (int) (seconde/60);
    seconde -= min*60;
    double dSec = (int) (seconde/10);
    seconde -= dSec*10;
    double sec = seconde;

    // Conversion du double sec en QString
    musicLengthSec = QString::number(sec);
    musicLengthDSec = QString::number(dSec);
    musicLengthMin = QString::number(min);
    musicLengthDMin = QString::number(dMin);

    refreshTimer();
    std::cout << "slideBar postionnée" << std::endl;
    std::cout << "fin du open" << std::endl;
}

void Player::play()
{
    std::cout << "début de lire" << std::endl;
    if(sonEnCours == true) // Si un son est en cours de lecture...
    {
        std::cout << "un son est en cours de lecture" << std::endl;
        bool isPaused;
        channel->getPaused(&isPaused);
        if (isPaused) // Si la chanson est en pause
        {
            std::cout << "le son est en pause" << std::endl;
            channel->setPaused(false);
            paused = false;
            //unsigned int musicLength;
            //sound->getLength(&musicLength,FMOD_TIMEUNIT_MS);
            //slideBar->setRange(0,musicLength);
            timer->start();
        }

        if (stop == true) // Si la chanson est en arrêt
        {
            if(isInVisualisationMode)
                emit refreshMusicWhileVisualisation(0);
            std::cout << "la chanson est en arret" << std::endl;
            system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
            paused = false;
            stop = false;
            image->setPixmap(QPixmap("images/play.png"));
            std::cout << "la musique est lancée" << std::endl;
            sec = 0;
            min = 0;
            d_sec = 0;
            d_min = 0;
            timer->start();
            std::cout << "chronos démarré" << std::endl;
            unsigned int musicLength;
            sound->getLength(&musicLength,FMOD_TIMEUNIT_MS);
            slideBar->setRange(0,musicLength);
            std::cout << "barre maj" << std::endl;
        }

    }

    if(sonEnCours == false) // Si aucun son n'est joué
    {

            open();
            system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
            paused = false;
            stop = false;


            std::cout << "son joué" << std::endl;
            sonEnCours = true;

            timer->start();

            std::cout << "chronos démarrés" << std::endl;
            sec = 0;
            min = 0;
            d_sec = 0;
            d_min = 0;

            infoMusique->setText("Lecture en cours: " + name);
            std::cout << "info et status maj" << std::endl;


    }

    infoMusique->setText("Lecture en cours :" + name);
    slideBar->setEnabled(true);
    image->setPixmap(QPixmap("images/play.png"));
}


void Player::refreshMusic(int newPosition)
{
    sec = newPosition/1000;
    unsigned int oldPosition;
    channel->getPosition(&oldPosition, FMOD_TIMEUNIT_MS);
    channel->setPosition(newPosition, FMOD_TIMEUNIT_MS);

    bool end = false;
    if(!isInVisualisationMode){
        while(!end && !tempo.empty()){
            if(tempo.back().position > newPosition){
                std::cout << "remove "<< tempo.back().position << " pos and " << tempo.back().duration << " duration from vector"<< std::endl;
                tempo.pop_back();
            }
            else
                end = true;
        }
    }
    else{
        emit refreshMusicWhileVisualisation(newPosition);
    }

}


void Player::changePosStream(int value)
{
    channel->setPosition(value,FMOD_TIMEUNIT_MS);
    double seconde = (double) (value /= 1000);
    d_min = (int) (seconde/60)/10;
    seconde -= d_min*600;
    min = (int) (seconde/60);
    seconde -= min*60;
    d_sec = (int) (seconde/10);
    seconde -= d_sec*10;
    sec = seconde;

    // Conversion du double sec en QString
    tempsEcouleSec = QString::number(sec);
    tempsEcouleDSec = QString::number(d_sec);
    tempsEcouleMin = QString::number(min);
    tempsEcouleDMin = QString::number(d_min);

    compteurTemps->setText(tempsEcouleDMin + tempsEcouleMin + ":" + tempsEcouleDSec + tempsEcouleSec + "/" + musicLengthDMin + musicLengthMin + ":" + musicLengthDSec + musicLengthSec);

}
void Player::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == 32)
        led->setPixmap(QPixmap("images/ledvert.png"));
}

void Player::keyPressEvent(QKeyEvent* event){
    switch(event->key()){
    case 32:
        led->setPixmap(QPixmap("images/ledrouge.png"));
        unsigned int pos;
        channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
        accordTime accord;
        accord.position = pos;
        if(tempo.empty())
            accord.duration = pos;
        else
            accord.duration = pos - tempo.back().position;

        std::cout << "add "<< accord.position << " pos and " << accord.duration << " duration to vector"<< std::endl;
        tempo.push_back(accord);
        break;
    case 80:
        play();
        break;
    case 83:
        pause();
        break;
    case 16777220:
        if(paused || stop)
            play();
        else
            pause();
        break;
    default:
        break;
    }
}

void Player::pause()
{
        if(sonEnCours == true) // Si un son est en cours de lecture...
        {
            bool isPlaying;
            channel->isPlaying(&isPlaying);
                if (isPlaying) // Si la chanson n'est pas en pause
                {
                    channel->setPaused(true);
                    timer->stop();
                    paused = true;
                }
        }
        image->setPixmap(QPixmap("images/pause.png"));
}

QAction* Player::getExportAction(){
    return exportData;
}

QAction* Player::getOpenAction(){
    return openAction;
}

QPushButton* Player::getVisualisationButton(){
    return visualisation;
}

void Player::visualiser(){
    if(!isInVisualisationMode){
        unsigned int position;
        channel->getPosition(&position, FMOD_TIMEUNIT_MS);
        changePosStream(position);
        isInVisualisationMode = true;
        visualisation->setText(QString("STOP VISUALISATION"));
        thread = new VisualisationThread(this);
        QObject::connect(thread, SIGNAL(ledMustBeRed()), this, SLOT(setLedRed()));
        QObject::connect(thread,SIGNAL(ledMustBeGreen()), this, SLOT(setLedGreen()));
        QObject::connect(this,SIGNAL(refreshMusicWhileVisualisation(int)),thread,SLOT(refreshMusic(int)));
        QObject::connect(thread,SIGNAL(terminated()), thread, SLOT(quit()));
        thread->start();
        emit refreshMusicWhileVisualisation(position);
    }
    else{
        isInVisualisationMode = false;
        visualisation->setText(QString("VISUALISATION"));
        thread->endThread();
        delete thread;
    }
}

void Player::setLedRed(){
    led->setPixmap(QPixmap("images/ledrouge.png"));
    led->update();
    std::cout << "je flash" << std::endl;
}

void Player::setLedGreen(){
    led->setPixmap(QPixmap("images/ledvert.png"));
    led->update();
}
