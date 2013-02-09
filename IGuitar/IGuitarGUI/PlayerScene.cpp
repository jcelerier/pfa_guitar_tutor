#include "PlayerScene.h"
#include "myitem.h"
#include "entiresong.h"
#include "config.h"
#include "MainWidget.hpp"

/* Scene principale du player
 */


PlayerScene::PlayerScene(QWidget *parent) :
    windowSize(Config::getWindowSize()),
    framesPerSecond(40),
    playing(false)
{
    this->parent = parent;
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

    // Container, petit probleme, desactivé pour le moment
    /*QGraphicsRectItem* forAllContainer = new QGraphicsRectItem(QRect(0,0,30,30));
    forAllContainer->setBrush(QBrush(Qt::red));

    QGraphicsOpacityEffect* forAllMasking = new QGraphicsOpacityEffect();
    QLinearGradient bgContour(0,0, Config::width/4, Config::height/4);
    bgContour.setColorAt(0, Qt::white);
    bgContour.setColorAt(1, Qt::white);
    QBrush forAllMask(bgContour);
    forAllMasking->setOpacity(1);
    forAllMasking->setOpacityMask(forAllMask);
    forAllContainer->setGraphicsEffect(forAllMasking);
    addItem(forAllContainer);*/


    // Image de fond
    QPixmap bgImg("bgwide.png");
    itemMap["backgnd"] = addPixmap(bgImg);


    // Bouton de menu
    QPixmap menuBtnImage("menu-btn.png");
    itemMap["menu"] = new myItem(menuBtnImage);
    addItem(itemMap["menu"]);
    itemMap["menu"]->setPos(820, 122); // Position absolue par rapport au background

    // Bouton de lecture/pause
    QPixmap transportImage("transport.png");
    itemMap["transport"] = new myItem(transportImage);
    addItem(itemMap["transport"]);
    itemMap["transport"]->setPos(40, 760); // Position absolue par rapport au background


    // Titre de la chanson
    itemMap["songTitle"] = addText("a", robotoFont);
    itemMap["songTitle"]->setPos(200, 65);
    ((QGraphicsTextItem*)itemMap["songTitle"])->setTextWidth(520);
    ((QGraphicsTextItem*)itemMap["songTitle"])->setHtml("<p align=\"center\">FreeBird</p>");

    // Artiste de la chanson
    itemMap["songArtist"] = addText("a", robotoFont);
    itemMap["songArtist"]->setPos(200, 115);
    ((QGraphicsTextItem*)itemMap["songArtist"])->setTextWidth(520);
    ((QGraphicsTextItem*)itemMap["songArtist"])->setHtml("<p align=\"center\">Lynyrd Skynyrd</p>"); // Useless ?

    // Conteneur pour les accords défilants
    QPixmap playingMask("maskplaying.png");
    //QGraphicsPolygonItem* textmContainer = new QGraphicsPolygonItem();
    QGraphicsPixmapItem* playingTextContainer = new myItem(playingMask);
    playingTextContainer->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    playingTextContainer->setOpacity(0.01); // Pour ne pas voir le masque mais qu'il ne soit pas desactivé
    addItem(playingTextContainer);
/*    QBrush defilBrush(Qt::blue);
    QGraphicsItem* defiltContainer = addRect(80,390,965, 185, QPen(), defilBrush);
*/

    // Masque pour cacher ce qui est hors du cadre
    QGraphicsOpacityEffect* textMasking = new QGraphicsOpacityEffect();
    QBrush brushMask(playingMask);
    textMasking->setOpacity(0.9);
    textMasking->setOpacityMask(brushMask);
    playingTextContainer->setGraphicsEffect(textMasking);

    // Accords defilants
    QFont liveChordsFont("Roboto", 30);
    liveChordsFont.setWordSpacing(50);
    QGraphicsTextItem *texteAccords = new QGraphicsTextItem(playingTextContainer);  //  Fils du container pour heriter du masque
    texteAccords->setFont(liveChordsFont);
    texteAccords->setPlainText("dList"); // Liste d'accords temporaire
    texteAccords->setPlainText(((MainWidget*) parent)->getChords()); // Liste d'accords temporaire
    texteAccords->setPos(90, 420);
    texteAccords->setDefaultTextColor(Qt::red);
    //addItem(texteAccords);

    itemMap["texteAccords"] = texteAccords;

    // Chansonreère
/*
    QPen borderPen(Qt::black, 3);
    QPen lightRPen(Qt::black, 1);
    QBrush innerCont(Qt::white);

    QGraphicsItem* lineCont = addRect(1470, 108, 355, 53, borderPen, innerCont);

    QList<QGraphicsItem*> chorditemlist;

    QGraphicsItem* chordU = addRect(1470, 108, 825, 150, borderPen, innerCont);
*/
    addItem(new EntireSong(((MainWidget*) parent)->getChordList()));

    // Accord joue a la guitare
    QFont playedFont("Roboto", 200);
    itemMap["chordPlayed"] = addText("0", playedFont);
    ((QGraphicsTextItem*)itemMap["chordPlayed"])->setDefaultTextColor(QColor(0, 161, 42));
    itemMap["chordPlayed"]->setPos(1180, 380);

    qDebug() << "les poils";

}

QGraphicsItem* PlayerScene::getItem(QString name) {
    return itemMap[name];
}

void PlayerScene::mousePressEvent(QGraphicsSceneMouseEvent*e)
{
    QGraphicsScene::mousePressEvent(e);

    if(e->buttons() & Qt::LeftButton)
    {
        if(mouseGrabberItem() == itemMap["menu"])
        {
            qDebug() << "menu btn touched";
            qApp->exit();
        }
        else if(mouseGrabberItem() == itemMap["songTitle"]) // Ne marche pas ...
        {
            qDebug() << "eu touched";
            ((QGraphicsTextItem*)itemMap["songTitle"])->setPlainText("teton du zboob");
        }
        else if(mouseGrabberItem() == itemMap["transport"])
        {
            qDebug() << "play/pause";
            switchPlaying();
        }
    }
}

void PlayerScene::switchPlaying()
{
    playing = !playing;
}

void PlayerScene::updateScene()
{
    //qDebug() << "updating scene";
    if(playing) {
        QTransform textTrans;
        textTrans.translate(-1,0);
        itemMap["texteAccords"]->setTransform(textTrans, true);
    }
}

void PlayerScene::newChordPlayed(QString chord) {
    ((QGraphicsTextItem*)itemMap["chordPlayed"])->setPlainText(chord);
}
