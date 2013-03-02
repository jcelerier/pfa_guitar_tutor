#include "PlayerScene.h"
#include "Controler.hpp"

/* Scene principale du player
 */


PlayerScene::PlayerScene(QObject *parent) :
    QGraphicsScene(parent),
    windowSize(Configuration::getWindowSize()),
    playing(false)
{
    controler = (Controler*)parent;
    disposeScene();

    resetNoteCheck();
}

void PlayerScene::disposeScene()
{
    //setSceneRect(0,0,1920,1080);
    // Police
    QFont robotoFont("Roboto",20);

    // Couleur de fond
    QColor bgColor(34, 14, 30);
    setBackgroundBrush(bgColor);

    // Image de fond
    QPixmap bgImg("bgwide.png");
    itemMap["backgnd"] = addPixmap(bgImg);
    ((QGraphicsPixmapItem*) itemMap["backgnd"])->setTransformationMode(Qt::SmoothTransformation);

    // Bouton de menu
	QPixmap menuBtnImage("menu.png");
    itemMap["menuBtn"] = new ButtonItem(menuBtnImage, itemMap["backgnd"]);
    itemMap["menuBtn"]->setPos(820, 122); // Position absolue par rapport au background
	itemMap["menuBtn"]->setToolTip("Menu");
    connect((ButtonItem*)itemMap["menuBtn"], SIGNAL(pushed()), this, SLOT(switchMenu()));

    // Bouton de lecture/pause
    QPixmap transportImage("transport.png");
    itemMap["transport"] = new ButtonItem(transportImage, itemMap["backgnd"]);
    itemMap["transport"]->setPos(40, 760); // Position absolue par rapport au background
    itemMap["transport"]->setToolTip("Play/Pause");
    connect((ButtonItem*)itemMap["transport"], SIGNAL(pushed()), this, SLOT(switchPlaying()));

    // Titre de la chanson
    itemMap["songTitle"] = addText("a", robotoFont);
    itemMap["songTitle"]->setPos(200, 65);
    ((QGraphicsTextItem*)itemMap["songTitle"])->setTextWidth(520);
    ((QGraphicsTextItem*)itemMap["songTitle"])->setHtml("<p align=\"center\">"+controler->track->getTrackName()+"</p>");

    // Artiste de la chanson
    itemMap["songArtist"] = addText("a", robotoFont);
    itemMap["songArtist"]->setPos(200, 115);
    ((QGraphicsTextItem*)itemMap["songArtist"])->setTextWidth(520);
    ((QGraphicsTextItem*)itemMap["songArtist"])->setHtml("<p align=\"center\">"+controler->track->getArtist()+"</p>");

    // Couverture d'album
    QPixmap albumImage("noalbum.png");
    itemMap["songAlbumImg"] = new QGraphicsPixmapItem(albumImage, itemMap["backgnd"]);
    itemMap["songAlbumImg"]->setPos(58,63);

    // Chanson entière

    itemMap["entireSong"] = new EntireSong(itemMap["backgnd"]);

    // Barre avancement
    QPixmap avancemntImage("barretemps.png");
    itemMap["avancmt"] = new QGraphicsPixmapItem(avancemntImage, itemMap["backgnd"]);
    itemMap["avancmt"]->setPos(200, 400); // Position absolue par rapport au background

    // Accord joue a la guitare
    QFont playedFont("Roboto", 200);
    itemMap["chordPlayed"] = addText("0", playedFont);
    ((QGraphicsTextItem*)itemMap["chordPlayed"])->setDefaultTextColor(QColor(0, 161, 42));
    itemMap["chordPlayed"]->setPos(1180, 380);

    // Menu
    itemMap["menu"] = new MenuItem(itemMap["backgnd"]);
    itemMap["menu"]->setVisible(false);
    itemMap["menu"]->setPanelModality(QGraphicsItem::PanelModal);


    // Options
    itemMap["options"] = new ButtonItem(menuBtnImage, itemMap["backgnd"]);
    itemMap["options"]->setPos(400, 760); // Position absolue par rapport au background
    itemMap["options"]->setToolTip("Audio options");
    connect((ButtonItem*)itemMap["options"], SIGNAL(pushed()), this, SLOT(openAudioOptions()));

}

QGraphicsItem* PlayerScene::getItem(QString name) {
    return itemMap[name];
}

void PlayerScene::mousePressEvent(QGraphicsSceneMouseEvent*e)
{
    QGraphicsScene::mousePressEvent(e);
}

void PlayerScene::switchPlaying()
{
    playing = !playing;
    controler->startClock();
}

void PlayerScene::openAudioOptions()
{
    Configuration config;
    addWidget(new AudioConfiguration(config));
}

void PlayerScene::switchMenu()
{
    itemMap["menu"]->setVisible(!itemMap["menu"]->isVisible());
}

void PlayerScene::updateScene()
{
    //qDebug() << "updating scene";
    if(playing) {
        /*QTransform textTrans;
        textTrans.translate(-1,0);
        itemMap["texteAccords"]->setTransform(textTrans, true);*/
        advance();
    }
}

void PlayerScene::setPlayedChord(QString playedChord) {
    ((QGraphicsTextItem*)itemMap["chordPlayed"])->setPlainText(playedChord);
    if(playedChord == ((EntireSong*)itemMap["entireSong"])->getCurrentChord()) {
        timeNoteSynchronized += (controler->elapsedTime() - lastTimeCheck);
        if (((timeNoteSynchronized * 100.0)/currentNoteDuration) > 30 /*Adaptative*/) {
            setCurrentChordValidated(true);
        }
    }
    lastTimeCheck = controler->elapsedTime();
}

void PlayerScene::resetNoteCheck()
{
    valideNote = false;
    timeNoteSynchronized = 0;
    currentNoteDuration = 2000;//((EntireSong*)itemMap["entireSong"])->getCurrentDuration();
}

void PlayerScene::setCurrentChordValidated(bool v)
{
    ((EntireSong*)itemMap["entireSong"])->validateChord(v);
}
