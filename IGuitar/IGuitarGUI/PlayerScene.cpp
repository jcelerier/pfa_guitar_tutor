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
	windowSize(Configuration::getWindowSize()),
	playing(false)
{
	controler = (Controler*)parent;
	disposeScene();

	resetNoteCheck();

    dictionary = new ChordDictionary(controler->getChordList());
}

/**
 * @brief PlayerScene::~PlayerScene
 *
 * Destructeur.
 */
PlayerScene::~PlayerScene()
{
    delete dictionary;
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
	QFont robotoFont("Roboto",20);

	// Couleur de fond
	QColor bgColor(34, 14, 30);
	setBackgroundBrush(bgColor);

	// Image de fond
	QPixmap bgImg(":/images/bgwide.png");
	itemMap["backgnd"] = addPixmap(bgImg);
	((QGraphicsPixmapItem*) itemMap["backgnd"])->setTransformationMode(Qt::SmoothTransformation);

	// Bouton de menu
	QPixmap menuBtnImage(":/images/menu.png");
	itemMap["menuBtn"] = new ButtonItem(menuBtnImage, itemMap["backgnd"]);
	itemMap["menuBtn"]->setPos(820, 122); // Position absolue par rapport au background
    itemMap["menuBtn"]->setToolTip(tr("Menu"));
	connect((ButtonItem*)itemMap["menuBtn"], SIGNAL(pushed()), this, SLOT(switchMenu()));

	// Bouton de lecture/pause
	QPixmap transportImage(":/images/transport.png");
	itemMap["transport"] = new ButtonItem(transportImage, itemMap["backgnd"]);
	itemMap["transport"]->setPos(40, 760); // Position absolue par rapport au background
    itemMap["transport"]->setToolTip(tr("Play/Pause"));
	connect((ButtonItem*)itemMap["transport"], SIGNAL(pushed()), this, SLOT(switchPlaying()));

	// Titre de la chanson
	itemMap["songTitle"] = addText("a", robotoFont);
	itemMap["songTitle"]->setPos(200, 65);
	((QGraphicsTextItem*)itemMap["songTitle"])->setTextWidth(520);
    ((QGraphicsTextItem*)itemMap["songTitle"])->setHtml("<p align=\"center\">"+controler->getTrack()->getTrackName()+"</p>");

	// Artiste de la chanson
	itemMap["songArtist"] = addText("a", robotoFont);
	itemMap["songArtist"]->setPos(200, 115);
	((QGraphicsTextItem*)itemMap["songArtist"])->setTextWidth(520);
    ((QGraphicsTextItem*)itemMap["songArtist"])->setHtml("<p align=\"center\">"+controler->getTrack()->getArtist()+"</p>");

	// Couverture d'album
	QPixmap albumImage(":/images/noalbum.png");
	itemMap["songAlbumImg"] = new QGraphicsPixmapItem(albumImage, itemMap["backgnd"]);
	itemMap["songAlbumImg"]->setPos(58,63);

	// Chanson entière
	itemMap["entireSong"] = new EntireSong(itemMap["backgnd"]);

	// Barre avancement
	QPixmap avancemntImage(":/images/barretemps.png");
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

    // Statistiques
    itemMap["totalPlayed"] = addText("0", robotoFont);
    itemMap["totalPlayed"]->setPos(1340, 120);
    ((QGraphicsTextItem*)itemMap["totalPlayed"])->setDefaultTextColor(QColor(255,255,225));
    itemMap["totalValidated"] = addText("0", robotoFont);
    itemMap["totalValidated"]->setPos(1340, 170);
    ((QGraphicsTextItem*)itemMap["totalValidated"])->setDefaultTextColor(QColor(255,255,255));

    // Dictionnaire d'accords
    QPixmap dictionaryImage(":/images/dictionary.png");
    itemMap["dictionary"] = new ButtonItem(dictionaryImage, itemMap["backgnd"]);
    itemMap["dictionary"]->setPos(57, 650); // Position absolue par rapport au background
    itemMap["dictionary"]->setToolTip(tr("Chord dictionary"));
    connect((ButtonItem*)itemMap["dictionary"], SIGNAL(pushed()), this, SLOT(displayDictionary()));
}

/**
 * @brief PlayerScene::getItem
 * @param name Nom de l'élément à récupérer
 * @return L'élement de l'interface demandé.
 *
 * Accesseur pour les différents éléments constitutifs de l'interface.
 */
QGraphicsItem* PlayerScene::getItem(QString name) {
	return itemMap[name];
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

/**
 * @brief PlayerScene::switchPlaying
 *
 * Alterne entre lecture et pause.
 */
void PlayerScene::switchPlaying()
{
	playing = !playing;
	controler->startClock();
}

/**
 * @brief PlayerScene::switchMenu
 *
 * Affiche ou cache le menu.
 */
void PlayerScene::switchMenu()
{
	itemMap["menu"]->setVisible(!itemMap["menu"]->isVisible());
}

/**
 * @brief PlayerScene::updateScene
 *
 * Mise à jour de l'interface.
 */
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

/**
 * @brief PlayerScene::setPlayedChord
 * @param playedChord Accord reconnu actuellement
 *
 * Affiche l'accord joué par l'utilisateur sur l'interface et met à jour la durée de synchronisation.
 */
void PlayerScene::setPlayedChord(QString playedChord) {
	((QGraphicsTextItem*)itemMap["chordPlayed"])->setPlainText(playedChord);
    if(!((EntireSong*)itemMap["entireSong"])->getIsCurrentChordValidated() &&
            playedChord == ((EntireSong*)itemMap["entireSong"])->getCurrentChord()) {
		timeNoteSynchronized += (controler->elapsedTime() - lastTimeCheck);
		if (((timeNoteSynchronized * 100.0)/currentNoteDuration) > 30 /*Adaptative*/) {
			setCurrentChordValidated(true);
		}
	}
	lastTimeCheck = controler->elapsedTime();
}

/**
 * @brief PlayerScene::resetNoteCheck
 *
 * Remise à zéro des variables utilisées pour le calcul de la durée de synchronisation. Doit etre appelé à chaque changement d'accord dans la partition.
 */
void PlayerScene::resetNoteCheck()
{
	valideNote = false;
	timeNoteSynchronized = 0;
    currentNoteDuration = ((EntireSong*)itemMap["entireSong"])->getCurrentDuration();
}

/**
 * @brief PlayerScene::setCurrentChordValidated
 * @param v Vrai si et seulement si l'accord courant doit etre validé
 *
 * Demande la validation ou non de l'accord courant.
 */
void PlayerScene::setCurrentChordValidated(bool v)
{
	((EntireSong*)itemMap["entireSong"])->validateChord(v);
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
    ((QGraphicsTextItem*)itemMap["totalValidated"])->setPlainText(QString::number(validated));
    ((QGraphicsTextItem*)itemMap["totalPlayed"])->setPlainText(QString::number(played));
}

/**
 * @brief PlayerScene::displayDictionary
 *
 * Affiche le dictionnaire d'accords.
 */
void PlayerScene::displayDictionary()
{
    dictionary->show();
}
