#include "PlayerScene.h"
#include "Controler.hpp"
#include <QDebug>

/**
 * @brief PlayerScene::PlayerScene
 * @param parent Element parent
 *
 * Constructeur.
 */
PlayerScene::PlayerScene(QObject *parent) :
	QGraphicsScene(parent),
    m_windowSize(Configuration::getWindowSize()),
    m_isPlaying(false)
{
    m_controler = (Controler*)parent;

	disposeScene();

	resetNoteCheck();

    //m_dictionary = new ChordDictionary(m_controler->getChordList());

}

/**
 * @brief PlayerScene::~PlayerScene
 *
 * Destructeur.
 */
PlayerScene::~PlayerScene()
{
    delete m_dictionary;
    delete m_itemMap["menuBtn"];
    delete m_itemMap["transport"];
    delete m_itemMap["dictionary"];
    delete m_itemMap["songAlbumImg"];
    delete m_itemMap["entireSong"];
    delete m_itemMap["avancmt"];
    delete m_itemMap["menu"];
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
    m_itemMap["backgnd"] = addPixmap(bgImg);
    ((QGraphicsPixmapItem*) m_itemMap["backgnd"])->setTransformationMode(Qt::SmoothTransformation);

	// Bouton de menu
	QPixmap menuBtnImage(":/images/menu.png");
    m_itemMap["menuBtn"] = new ButtonItem(menuBtnImage, m_itemMap["backgnd"]);
    m_itemMap["menuBtn"]->setPos(820, 122); // Position absolue par rapport au background
    m_itemMap["menuBtn"]->setToolTip(tr("Menu"));
    connect((ButtonItem*)m_itemMap["menuBtn"], SIGNAL(pushed()), this, SLOT(switchMenu()));

	// Bouton de lecture/pause
	QPixmap transportImage(":/images/transport.png");
    m_itemMap["transport"] = new ButtonItem(transportImage, m_itemMap["backgnd"]);
    m_itemMap["transport"]->setPos(40, 760); // Position absolue par rapport au background
    m_itemMap["transport"]->setToolTip(tr("Play/Pause"));
    connect((ButtonItem*)m_itemMap["transport"], SIGNAL(pushed()), this, SLOT(switchPlaying()));

    // Dictionnaire d'accords
    QPixmap dictionaryImage(":/images/dictionary.png");
    m_itemMap["dictionary"] = new ButtonItem(dictionaryImage, m_itemMap["backgnd"]);
    m_itemMap["dictionary"]->setPos(57, 650); // Position absolue par rapport au background
    m_itemMap["dictionary"]->setToolTip(tr("Chord dictionary"));
    connect((ButtonItem*)m_itemMap["dictionary"], SIGNAL(pushed()), this, SLOT(displayDictionary()));

	// Titre de la chanson
    m_itemMap["songTitle"] = addText("a", robotoFont);
    m_itemMap["songTitle"]->setPos(200, 65);
    ((QGraphicsTextItem*)m_itemMap["songTitle"])->setTextWidth(520);
    ((QGraphicsTextItem*)m_itemMap["songTitle"])->setHtml("<p align=\"center\">"+m_controler->getTrack()->getTrackName()+"</p>");

	// Artiste de la chanson
    m_itemMap["songArtist"] = addText("a", robotoFont);
    m_itemMap["songArtist"]->setPos(200, 115);
    ((QGraphicsTextItem*)m_itemMap["songArtist"])->setTextWidth(520);
    ((QGraphicsTextItem*)m_itemMap["songArtist"])->setHtml("<p align=\"center\">"+m_controler->getTrack()->getArtist()+"</p>");

	// Couverture d'album
	QPixmap albumImage(":/images/noalbum.png");
    m_itemMap["songAlbumImg"] = new QGraphicsPixmapItem(albumImage, m_itemMap["backgnd"]);
    m_itemMap["songAlbumImg"]->setPos(58,63);

	// Chanson entière
    m_itemMap["entireSong"] = new EntireSong(m_itemMap["backgnd"]);

	// Barre avancement
	QPixmap avancemntImage(":/images/barretemps.png");
    m_itemMap["avancmt"] = new QGraphicsPixmapItem(avancemntImage, m_itemMap["backgnd"]);
    m_itemMap["avancmt"]->setPos(200, 400); // Position absolue par rapport au background

	// Accord joue a la guitare

    QFont playedFont("Roboto", 150);
    m_itemMap["chordPlayed"] = addText("0", playedFont);
    ((QGraphicsTextItem*)m_itemMap["chordPlayed"])->setDefaultTextColor(QColor(0, 161, 42));
    m_itemMap["chordPlayed"]->setPos(1150, 380);

	// Menu
    m_itemMap["menu"] = new MenuItem(m_itemMap["backgnd"]);
    m_itemMap["menu"]->setVisible(false);
    m_itemMap["menu"]->setPanelModality(QGraphicsItem::PanelModal);


	// Statistiques
    m_itemMap["totalPlayed"] = new QGraphicsTextItem("0", m_itemMap["backgnd"]);
    ((QGraphicsTextItem*)m_itemMap["totalPlayed"])->setFont(robotoFont);
    m_itemMap["totalPlayed"]->setPos(1340, 110);
    ((QGraphicsTextItem*)m_itemMap["totalPlayed"])->setDefaultTextColor(QColor(255,255,225));
    m_itemMap["totalValidated"] = addText("0", robotoFont);
    m_itemMap["totalValidated"]->setPos(1340, 170);
    ((QGraphicsTextItem*)m_itemMap["totalValidated"])->setDefaultTextColor(QColor(255,255,255));
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

/**
 * @brief PlayerScene::switchPlaying
 *
 * Alterne entre lecture et pause.
 */
void PlayerScene::switchPlaying()
{
    static bool isFirstPlay = true;
    m_isPlaying = !m_isPlaying;
    m_controler->switchPlaying();
    if(m_isPlaying && !isFirstPlay) {
        // Chanson entière
        delete m_itemMap["entireSong"];
        m_itemMap["entireSong"] = new EntireSong(m_itemMap["backgnd"]);
        updateStats(0, 0);
    }
    isFirstPlay = false;
}

/**
 * @brief PlayerScene::switchMenu
 *
 * Affiche ou cache le menu.
 */
void PlayerScene::switchMenu()
{
    m_itemMap["menu"]->setVisible(!m_itemMap["menu"]->isVisible());
}

/**
 * @brief PlayerScene::updateScene
 *
 * Mise à jour de l'interface.
 */
void PlayerScene::updateScene()
{
    if(m_isPlaying)
	{
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
void PlayerScene::setPlayedChord(QStringList playedChord) {
    if(!((EntireSong*)m_itemMap["entireSong"])->getIsCurrentChordValidated() &&
            playedChord.contains(((EntireSong*)m_itemMap["entireSong"])->getCurrentChord())) {
        m_timeNoteSynchronized += (m_controler->elapsedTime() - m_lastTimeCheck);
        if (((m_timeNoteSynchronized * 100.0)/m_currentNoteDuration) > 30 /*Adaptative*/) {
			setCurrentChordValidated(true);
		}
        //Affichage de la note attendue
        //((QGraphicsTextItem*)itemMap["chordPlayed"])->setHtml(playedChord[0]+"<sub>"+playedChord.mid(1)+"</sub>");
        ((QGraphicsTextItem*)m_itemMap["chordPlayed"])->setPlainText(playedChord.at(playedChord.indexOf(((EntireSong*)m_itemMap["entireSong"])->getCurrentChord())));
	}
    else //Affichage d'une note au hasard parmis les résultats possibles
        ((QGraphicsTextItem*)m_itemMap["chordPlayed"])->setPlainText(playedChord.at(0));
    m_lastTimeCheck = m_controler->elapsedTime();
}

/**
 * @brief PlayerScene::resetNoteCheck
 *
 * Remise à zéro des variables utilisées pour le calcul de la durée de synchronisation. Doit etre appelé à chaque changement d'accord dans la partition.
 */
void PlayerScene::resetNoteCheck()
{
    m_timeNoteSynchronized = 0;
    m_currentNoteDuration = ((EntireSong*)m_itemMap["entireSong"])->getCurrentDuration();
}

/**
 * @brief PlayerScene::setCurrentChordValidated
 * @param v Vrai si et seulement si l'accord courant doit etre validé
 *
 * Demande la validation ou non de l'accord courant.
 */
void PlayerScene::setCurrentChordValidated(bool v)
{
    ((EntireSong*)m_itemMap["entireSong"])->validateChord(v);
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
    ((QGraphicsTextItem*)m_itemMap["totalValidated"])->setPlainText(QString::number(validated));
    ((QGraphicsTextItem*)m_itemMap["totalPlayed"])->setPlainText(QString::number(played));
}

/**
 * @brief PlayerScene::displayDictionary
 *
 * Affiche le dictionnaire d'accords.
 */
void PlayerScene::displayDictionary()
{
    //m_dictionary->show();
}

Controler* PlayerScene::getControler() {
    return m_controler;
}

