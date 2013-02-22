#include "PlayerScene.h"
#include "Controler.hpp"

/* Scene principale du player
 */


PlayerScene::PlayerScene(QObject *parent) :
    QGraphicsScene(parent),
    windowSize(Configuration::getWindowSize()),
    framesPerSecond(40),
    playing(false)
{
    controler = (Controler*)parent;
    disposeScene();
    QTimer *t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    t_Timer->start(1000/framesPerSecond);
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
    ((QGraphicsTextItem*)itemMap["songTitle"])->setHtml("<p align=\"center\">FreeBird</p>");

    // Artiste de la chanson
    itemMap["songArtist"] = addText("a", robotoFont);
    itemMap["songArtist"]->setPos(200, 115);
    ((QGraphicsTextItem*)itemMap["songArtist"])->setTextWidth(520);
    ((QGraphicsTextItem*)itemMap["songArtist"])->setHtml("<p align=\"center\">Lynyrd Skynyrd</p>");

    // Couverture d'album
    QPixmap albumImage("noalbum.png");
    itemMap["songAlbumImg"] = new QGraphicsPixmapItem(albumImage, itemMap["backgnd"]);
    itemMap["songAlbumImg"]->setPos(58,63);

    // Chanson entière

    itemMap["entireSong"] = new EntireSong(itemMap["backgnd"]);
    addItem(itemMap["entireSong"]);

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

void PlayerScene::newChordPlayed(QString chord) {
    ((QGraphicsTextItem*)itemMap["chordPlayed"])->setPlainText(chord);
}
