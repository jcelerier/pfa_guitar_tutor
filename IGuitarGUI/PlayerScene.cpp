#include "PlayerScene.h"
#include "Controler.hpp"

/**
 * @brief PlayerScene::PlayerScene
 * @param parent Element parent
 *
 * Constructeur.
 */
PlayerScene::PlayerScene(QObject *parent) :
    QGraphicsScene(parent),
    m_windowSize(Configuration::getWindowSize()),
    m_cntdownOver(false),
    m_cntdown(0),
    m_isPlaying(false),
    m_lastChord(0),
    m_loaded(false)
{
    m_controler = (Controler*)parent;

    m_cntTimer = new QTimer(this);
    connect(m_cntTimer, SIGNAL(timeout()), this, SLOT(playCountdown()));

    disposeScene();

    m_cntClick = new QSoundEffect();
    m_cntClick->setSource(QUrl("qrc:/sounds/Metronome.wav"));
    m_cntClick->setVolume(0.60f);
    m_cntClickUp = new QSoundEffect();
    m_cntClickUp->setSource(QUrl("qrc:/sounds/MetronomeUp.wav"));
    m_cntClickUp->setVolume(0.60f);

    m_dictionary = new ChordDictionary();
    Configuration* config = m_controler->getConfiguration();
    m_configPanel = new ConfigPanel(config->getLoopSetting(), config->getDifficulty(), config->getPauseSetting());
    connect(m_configPanel, SIGNAL(configChanged(bool,int,int)), this, SLOT(updateConfiguration(bool,int,int)));

}

/**
 * @brief PlayerScene::~PlayerScene
 *
 * Destructeur.
 */
PlayerScene::~PlayerScene()
{
    delete m_dictionary;
    delete m_configPanel;
    delete m_cntTimer;

    delete m_itemMap["backgnd"];
}

/**
 * @brief PlayerScene::disposeScene
 *
 * Mise en place des différents éléments constitutifs de l'interface.
 */
void PlayerScene::disposeScene()
{
    //setSceneRect(0,0,1920,1080);
    // Police
    QFont titleFont("Roboto" ,20);

    // Couleur de fond
    QColor bgColor(34, 14, 30);
    //setBackgroundBrush(bgColor);

    //Couches de masquage pour les accords defilants
    m_itemMap["maskPlay"] = addPixmap(QPixmap(":/images/maskplaying.png"));
    ((QGraphicsPixmapItem*) m_itemMap["maskPlay"])->setTransformationMode(Qt::SmoothTransformation);
    ((QGraphicsPixmapItem*) m_itemMap["maskPlay"])->setPos(0,0);

    m_itemMap["maskScroll"] = addPixmap(QPixmap(":/images/maskscrolling.png"));
    ((QGraphicsPixmapItem*) m_itemMap["maskScroll"])->setTransformationMode(Qt::SmoothTransformation);

    // Image de fond
    m_itemMap["backgnd"] = addPixmap(QPixmap(":/images/bgwide.png"));
    ((QGraphicsPixmapItem*) m_itemMap["backgnd"])->setTransformationMode(Qt::SmoothTransformation);
    ((QGraphicsPixmapItem*) m_itemMap["backgnd"])->setPos(0,0);
    // Bouton de menu
    QPixmap menuBtnImage(":/images/menu.png");
    m_itemMap["menuBtn"] = new ButtonItem(menuBtnImage, m_itemMap["backgnd"]);
    m_itemMap["menuBtn"]->setPos(820, 122); // Position absolue par rapport au background
    m_itemMap["menuBtn"]->setToolTip(tr("Menu"));
    connect((ButtonItem*)m_itemMap["menuBtn"], SIGNAL(pushed()), this, SLOT(switchMenu()));

    // Transport
    QPixmap playImage(":/images/play.png");
    QPixmap pauseImage(":/images/pause.png");
    QPixmap stopImage(":/images/stop.png");
    QPixmap backImage(":/images/back.png");

    m_itemMap["play"] = new ButtonItem(playImage, m_itemMap["backgnd"]);
    m_itemMap["play"]->setPos(40, 860); // Position absolue par rapport au background
    m_itemMap["play"]->setToolTip(tr("Play"));
    connect((ButtonItem*)m_itemMap["play"], SIGNAL(pushed()), this, SLOT(play()));

    m_itemMap["pause"] = new ButtonItem(pauseImage, m_itemMap["backgnd"]);
    m_itemMap["pause"]->setPos(170, 860); // Position absolue par rapport au background
    m_itemMap["pause"]->setToolTip(tr("Pause"));
    connect((ButtonItem*)m_itemMap["pause"], SIGNAL(pushed()), this, SLOT(pause()));

    m_itemMap["stop"] = new ButtonItem(stopImage, m_itemMap["backgnd"]);
    m_itemMap["stop"]->setPos(300, 860); // Position absolue par rapport au background
    m_itemMap["stop"]->setToolTip(tr("Stop"));
    connect((ButtonItem*)m_itemMap["stop"], SIGNAL(pushed()), this, SLOT(stop()));

    m_itemMap["back"] = new ButtonItem(backImage, m_itemMap["backgnd"]);
    m_itemMap["back"]->setPos(470, 860); // Position absolue par rapport au background
    m_itemMap["back"]->setToolTip(tr("Back"));
    connect((ButtonItem*)m_itemMap["back"], SIGNAL(pushed()), this, SLOT(back()));


    // Dictionnaire d'accords
    QPixmap dictionaryImage(":/images/dictionary.png");
    m_itemMap["dictionary"] = new ButtonItem(dictionaryImage, m_itemMap["backgnd"]);
    m_itemMap["dictionary"]->setPos(57, 650); // Position absolue par rapport au background
    m_itemMap["dictionary"]->setToolTip(tr("Chord dictionary"));
    connect((ButtonItem*)m_itemMap["dictionary"], SIGNAL(pushed()), this, SLOT(displayDictionary()));

    // Bouton de mute
    QPixmap muteImage(":/images/unMute.png");
    m_itemMap["mute"] = new ButtonItem(muteImage, m_itemMap["backgnd"]);
    m_itemMap["mute"]->setPos(220, 650); // Position absolue par rapport au background
    m_itemMap["mute"]->setToolTip(tr("Mute/Unmute"));
    connect((ButtonItem*)m_itemMap["mute"], SIGNAL(pushed()), this, SLOT(switchMute()));

    // Titre de la chanson
    m_itemMap["songTitle"] = new QGraphicsTextItem("", m_itemMap["backgnd"]);
    m_itemMap["songTitle"]->setPos(200, 65);
    ((QGraphicsTextItem*)m_itemMap["songTitle"])->setTextWidth(520);
    ((QGraphicsTextItem*)m_itemMap["songTitle"])->setFont(titleFont);

    // Artiste de la chanson
    m_itemMap["songArtist"] = new QGraphicsTextItem("", m_itemMap["backgnd"]);
    m_itemMap["songArtist"]->setPos(200, 115);
    ((QGraphicsTextItem*)m_itemMap["songArtist"])->setTextWidth(520);
    ((QGraphicsTextItem*)m_itemMap["songArtist"])->setFont(titleFont);

    // Commentaire de la chanson
    m_itemMap["songComment"] = new QGraphicsTextItem("", m_itemMap["backgnd"]);
    m_itemMap["songComment"]->setPos(200, 165);
    ((QGraphicsTextItem*)m_itemMap["songComment"])->setTextWidth(520);
    ((QGraphicsTextItem*)m_itemMap["songComment"])->setFont(titleFont);

    // Couverture d'album
    QPixmap albumImage(":/images/noalbum.png");
    m_itemMap["songAlbumImg"] = new QGraphicsPixmapItem(albumImage, m_itemMap["backgnd"]);
    m_itemMap["songAlbumImg"]->setPos(58,63);

    // Barre avancement
    QPixmap avancemntImage(":/images/barretemps.png");
    m_itemMap["avancmt"] = new QGraphicsPixmapItem(avancemntImage, m_itemMap["backgnd"]);
    m_itemMap["avancmt"]->setPos(200, 400); // Position absolue par rapport au background

    // Accord joue a la guitare
    QFont playedFont("Roboto", 130/*150*/);
    m_itemMap["chordPlayed"] = new QGraphicsTextItem("", m_itemMap["backgnd"]);
    ((QGraphicsTextItem*)m_itemMap["chordPlayed"])->setFont(playedFont);
    ((QGraphicsTextItem*)m_itemMap["chordPlayed"])->setDefaultTextColor(QColor(0, 161, 42));
    m_itemMap["chordPlayed"]->setPos(1110, 380);

    // Statistiques
    QFont statsFont("Roboto",28);
    m_itemMap["totalPlayed"] = new QGraphicsTextItem("", m_itemMap["backgnd"]);
    ((QGraphicsTextItem*)m_itemMap["totalPlayed"])->setFont(statsFont);
    m_itemMap["totalPlayed"]->setPos(1210, 125);
    ((QGraphicsTextItem*)m_itemMap["totalPlayed"])->setTextWidth(100);
    ((QGraphicsTextItem*)m_itemMap["totalPlayed"])->setDefaultTextColor(QColor(101, 215, 78));

    m_itemMap["totalValidated"] = new QGraphicsTextItem("", m_itemMap["backgnd"]);
    ((QGraphicsTextItem*)m_itemMap["totalValidated"])->setFont(statsFont);
    m_itemMap["totalValidated"]->setPos(1210, 210);
    ((QGraphicsTextItem*)m_itemMap["totalValidated"])->setTextWidth(100);
    ((QGraphicsTextItem*)m_itemMap["totalValidated"])->setDefaultTextColor(QColor(101, 215, 78));

    // Decompte de lecture
    QFont countFont("Roboto", 90);
    m_itemMap["countDown"] = new QGraphicsTextItem("", m_itemMap["backgnd"]);
    ((QGraphicsTextItem*)m_itemMap["countDown"])->setFont(countFont);
    ((QGraphicsTextItem*)m_itemMap["countDown"])->setPos(300-45, 680-45);
    ((QGraphicsTextItem*)m_itemMap["countDown"])->setDefaultTextColor(Qt::white);


    // Menu
    m_itemMap["menu"] = new MenuItem(m_itemMap["backgnd"]);
    m_itemMap["menu"]->setVisible(false);
    m_itemMap["menu"]->setPanelModality(QGraphicsItem::SceneModal);
}

/**
 * @brief PlayerScene::getItem
 * @param name Nom de l'élément à récupérer
 * @return L'élement de l'interface demandé.
 *
 * Accesseur pour les différents éléments constitutifs de l'interface.
 */
QGraphicsItem* PlayerScene::getItem(QString name) {
    return m_itemMap[name];
}

/**
 * @brief PlayerScene::mousePressEvent
 * @param e Evènement
 *
 * Se déclenche lors d'un clic souris.
 */
void PlayerScene::mousePressEvent(QGraphicsSceneMouseEvent*e)
{
    QGraphicsScene::mousePressEvent(e);
}


void PlayerScene::play()
{
    if(m_itemMap["menu"]->isVisible())
        return;
    int cntTime;
    if(m_loaded) {
        if(!m_isPlaying) {
            if(!m_cntdownOver) {
                m_cntdown = 4;
                playCountdown();

                cntTime=1000;
                if(m_controler->getTrack()->getBPM() != 0)
                    cntTime = 60000/m_controler->getTrack()->getBPM();
                m_cntTimer->start(cntTime);

                return;
            }
            if(m_cntdownOver)
                m_cntdownOver = false;

            m_isPlaying = true;
            m_controler->startSong();
        }
    }
    else
        m_controler->initSong();

}
void PlayerScene::pause()
{
    if(m_isPlaying) {
        m_isPlaying = false;
        m_controler->pauseSong();
    }
}
void PlayerScene::stop()
{
    if(m_itemMap["menu"]->isVisible())
        return;

    if(m_cntdown>0)
    {
        m_cntTimer->stop();
        ((QGraphicsTextItem*)m_itemMap["countDown"])->setPlainText("");
    }
    m_isPlaying = false;
    m_controler->stopSong();
}
void PlayerScene::back()
{
    if(m_itemMap["menu"]->isVisible())
        return;
    stop();
    play();
}

void PlayerScene::switchMute()
{
    if(m_loaded) {
        QPixmap muteImage(":/images/mute.png");
        QPixmap unMuteImage(":/images/unMute.png");
        if(m_controler->muteState())
        {
            ((ButtonItem *)m_itemMap["mute"])->changeImage(unMuteImage);
            m_controler->unmute();
        }
        else
        {
            ((ButtonItem *)m_itemMap["mute"])->changeImage(muteImage);
            m_controler->mute();
        }
    }
}

void PlayerScene::switchPlay()
{
    if(m_itemMap["menu"]->isVisible())
        return;
    if(m_isPlaying)
    {
        pause();
    }
    else
    {
        play();
    }
}

/**
 * @brief PlayerScene::switchMenu
 *
 * Affiche ou cache le menu.
 */
void PlayerScene::switchMenu()
{
    switchMenu(!m_itemMap["menu"]->isVisible());
}

/**
 * @brief PlayerScene::switchMenu
 *
 * Affiche ou cache le menu. (surcharge)
 */
void PlayerScene::switchMenu(bool b)
{
    m_itemMap["menu"]->setVisible(b);
    pause();
}

/**
 * @brief PlayerScene::updateScene
 *
 * Mise à jour de l'interface.
 */
void PlayerScene::updateScene()
{
    if(m_isPlaying)
        advance();

}

void PlayerScene::updateConfiguration(bool isLoopingActive, int difficulty, int continueMode)
{
    m_controler->getConfiguration()->setDifficulty(difficulty);
    m_controler->getConfiguration()->setPauseSetting(continueMode);
    m_controler->getConfiguration()->setLoopSetting(isLoopingActive);
}

/**
 * @brief PlayerScene::setPlayedChord
 * @param playedChord Accord reconnu actuellement
 *
 * Affiche l'accord joué par l'utilisateur sur l'interface et met à jour la durée de synchronisation.
 */
void PlayerScene::setPlayedChord(BasicChord ch)
{
    QStringList playedChordList = BasicChord::convertChordToStringList(ch.toString());
    QString playedChord;

    if(playedChordList.contains(m_controler->getCurrentChord()->getChord())) {
        //Affichage de la note attendue
        //((QGraphicsTextItem*)itemMap["chordPlayed"])->setHtml(playedChord[0]+"<sub>"+playedChord.mid(1)+"</sub>");
        playedChord = m_controler->getCurrentChord()->getChord();
    }
    else //Affichage d'une note au hasard parmis les résultats possibles
        playedChord = playedChordList.at(0);

    ((QGraphicsTextItem*)m_itemMap["chordPlayed"])->setHtml(playedChord[0]+"<sub>"+playedChord.mid(1)+"</sub>");
}


/**
 * @brief PlayerScene::updateStats
 * @param validated Nombre de notes jouées et validées
 * @param played Nombre de notes jouées
 *
 * Met à jour les statistiques sur l'interface.
 */
void PlayerScene::updateStats(int validated, int played)
{
    ((QGraphicsTextItem*)m_itemMap["totalValidated"])->setHtml("<p align=\"center\">"+QString::number(validated)+"</p>");
    ((QGraphicsTextItem*)m_itemMap["totalPlayed"])->setHtml("<p align=\"center\">"+QString::number(played)+"</p>");
}

/**
 * @brief PlayerScene::displayDictionary
 *
 * Affiche le dictionnaire d'accords.
 */
void PlayerScene::displayDictionary()
{
    m_dictionary->show();
}

void PlayerScene::displayOptions() {

    m_configPanel->show();
}

Controler* PlayerScene::getControler() {
    return m_controler;
}

void PlayerScene::playCountdown() {
    QString num;
    if(m_cntdown > 0) {
        if(m_cntdown == 4)
            m_cntClickUp->play();
        else
            m_cntClick->play();
        num.setNum(m_cntdown);
        ((QGraphicsTextItem*)m_itemMap["countDown"])->setPlainText(num);
        m_cntdown--;
    }
    else {
        m_cntdownOver = true;
        m_cntTimer->stop();
        ((QGraphicsTextItem*)m_itemMap["countDown"])->setPlainText("");
        play();
    }
}


void PlayerScene::setSceneToChord(TrackChord* tc) {

    ((EntireSong*)m_itemMap["entireSong"])->setCurrentChord(tc);

    //if(tc->previous() != m_lastChord)
    ((ScrollingItem*)m_itemMap["scrollingChords"])->setCurrentChord(tc);
    m_lastChord = tc;
}

void PlayerScene::loadSong(LogicalTrack* track) {
    if(m_itemMap["entireSong"] != 0) {
        delete m_itemMap["entireSong"];
        m_itemMap["entireSong"] = 0;
    }
    if(m_itemMap["scrollingChords"] != 0) {
        delete m_itemMap["scrollingChords"];
        m_itemMap["scrollingChords"] = 0;
    }

    ((QGraphicsTextItem*)m_itemMap["songTitle"])->setHtml("<p align=\"center\">"+m_controler->getTrack()->getTrackName()+"</p>");
    ((QGraphicsTextItem*)m_itemMap["songArtist"])->setHtml("<p align=\"center\">"+m_controler->getTrack()->getArtist()+"</p>");
    ((QGraphicsTextItem*)m_itemMap["songComment"])->setHtml("<p align=\"center\">"+m_controler->getTrack()->getComment()+"</p>");
    if(QFile("albumcover.jpg").exists()) {
        QPixmap pixmap = QPixmap("albumcover.jpg").scaled(146,146);
        if(pixmap.isNull())
            pixmap = QPixmap(":/images/noalbum.png");
        ((QGraphicsPixmapItem*)m_itemMap["songAlbumImg"])->setPixmap(pixmap);
    }
    else {
        ((QGraphicsPixmapItem*)m_itemMap["songAlbumImg"])->setPixmap(QPixmap(":/images/noalbum.png"));
    }

    //Stats
    updateStats(0,0);

    // Chanson entière
    m_itemMap["entireSong"] = new EntireSong(m_itemMap["backgnd"]);
    ((EntireSong*) m_itemMap["entireSong"])->load(track);

    m_itemMap["scrollingChords"] = new ScrollingItem(m_itemMap["maskPlay"]);
    ((ScrollingItem*) m_itemMap["scrollingChords"])->load(track);

    m_dictionary->load(track);

    // Ordonnancement des couches
    m_itemMap["maskPlay"]->setZValue(0);
    m_itemMap["scrollingChords"]->setZValue(2);
    m_itemMap["maskScroll"]->setZValue(5);
    m_itemMap["backgnd"]->setZValue(15);
    m_itemMap["entireSong"]->setZValue(20);
    m_itemMap["menu"]->setZValue(50);
    switchMenu(false);

    m_loaded = true;
}
