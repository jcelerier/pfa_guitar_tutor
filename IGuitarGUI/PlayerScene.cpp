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
	m_isPlaying(false)
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

    m_dictionary = new ChordDictionary(m_controler->getChordList());
    m_configPanel = new ConfigPanel();

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
	QFont titleFont("Roboto",20);

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

	// Transport
	QPixmap playImage(":/images/play.png");
	QPixmap pauseImage(":/images/pause.png");
	QPixmap stopImage(":/images/stop.png");
	QPixmap backImage(":/images/back.png");

	m_itemMap["play"] = new ButtonItem(playImage, m_itemMap["backgnd"]);
	m_itemMap["play"]->setPos(40, 860); // Position absolue par rapport au background
	m_itemMap["play"]->setToolTip(tr("Play/Pause"));
	connect((ButtonItem*)m_itemMap["play"], SIGNAL(pushed()), this, SLOT(play()));

	m_itemMap["pause"] = new ButtonItem(pauseImage, m_itemMap["backgnd"]);
	m_itemMap["pause"]->setPos(170, 860); // Position absolue par rapport au background
	m_itemMap["pause"]->setToolTip(tr("Play/Pause"));
	connect((ButtonItem*)m_itemMap["pause"], SIGNAL(pushed()), this, SLOT(pause()));

	m_itemMap["stop"] = new ButtonItem(stopImage, m_itemMap["backgnd"]);
	m_itemMap["stop"]->setPos(300, 860); // Position absolue par rapport au background
	m_itemMap["stop"]->setToolTip(tr("Play/Pause"));
	connect((ButtonItem*)m_itemMap["stop"], SIGNAL(pushed()), this, SLOT(stop()));

	m_itemMap["back"] = new ButtonItem(backImage, m_itemMap["backgnd"]);
	m_itemMap["back"]->setPos(470, 860); // Position absolue par rapport au background
	m_itemMap["back"]->setToolTip(tr("Play/Pause"));
	connect((ButtonItem*)m_itemMap["back"], SIGNAL(pushed()), this, SLOT(back()));


	// Dictionnaire d'accords
	QPixmap dictionaryImage(":/images/dictionary.png");
	m_itemMap["dictionary"] = new ButtonItem(dictionaryImage, m_itemMap["backgnd"]);
	m_itemMap["dictionary"]->setPos(57, 650); // Position absolue par rapport au background
	m_itemMap["dictionary"]->setToolTip(tr("Chord dictionary"));
	connect((ButtonItem*)m_itemMap["dictionary"], SIGNAL(pushed()), this, SLOT(displayDictionary()));

	// Titre de la chanson
	m_itemMap["songTitle"] = addText("Title", titleFont);
	m_itemMap["songTitle"]->setPos(200, 65);
	((QGraphicsTextItem*)m_itemMap["songTitle"])->setTextWidth(520);

	// Artiste de la chanson
	m_itemMap["songArtist"] = addText("Artist", titleFont);
	m_itemMap["songArtist"]->setPos(200, 115);
	((QGraphicsTextItem*)m_itemMap["songArtist"])->setTextWidth(520);

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

/**
 * @brief PlayerScene::switchPlaying
 *
 * Alterne entre lecture et pause.
 */
void PlayerScene::switchPlaying()
{
	if(!m_isPlaying && !m_cntdownOver) {
		m_cntdown = 4;
		playCountdown();
		m_cntTimer->start(1000);
		return;
	}
	if(m_cntdownOver)
		m_cntdownOver = false;

	m_isPlaying = !m_isPlaying;
	m_controler->switchPlaying();
}


void PlayerScene::play()
{
	if(!m_isPlaying) {
		if(!m_cntdownOver) {
			m_cntdown = 4;
			playCountdown();
			m_cntTimer->start(1000);
			return;
		}
		if(m_cntdownOver)
			m_cntdownOver = false;

		m_isPlaying = true;
		m_controler->startSong();
	}
}
void PlayerScene::pause()
{
	m_isPlaying = false;
	m_controler->pauseSong();
}
void PlayerScene::stop()
{
	m_isPlaying = false;
	m_controler->stopSong();
}
void PlayerScene::back()
{
	m_isPlaying = false;
	m_controler->stopSong();
	play();
}

void PlayerScene::switchMute()
{
	if(m_controler->muteState())
	{
		m_controler->unmute();
	}
	else
	{
		m_controler->mute();
	}
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
		advance();

}

/**
 * @brief PlayerScene::setPlayedChord
 * @param playedChord Accord reconnu actuellement
 *
 * Affiche l'accord joué par l'utilisateur sur l'interface et met à jour la durée de synchronisation.
 */
void PlayerScene::setPlayedChord(BasicChord ch)
{
	QStringList playedChordList = BasicChord::convertChordToStringList(ch.toString().toLatin1());
	QString playedChord;
/*
	if(playedChordList.contains(((EntireSong*)m_itemMap["entireSong"])->getCurrentChord())) {
		//Affichage de la note attendue
		//((QGraphicsTextItem*)itemMap["chordPlayed"])->setHtml(playedChord[0]+"<sub>"+playedChord.mid(1)+"</sub>");
		playedChord = ((EntireSong*)m_itemMap["entireSong"])->getCurrentChord();
	}
	else //Affichage d'une note au hasard parmis les résultats possibles
		playedChord = playedChordList.at(0);
*/
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
    qDebug() << "Clic";
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
		switchPlaying();
	}
}


void PlayerScene::goToChord(TrackChord* tc) {
	int nChord = 0;

	for(int i=0; i<m_controler->getChordList()->size(); i++) {
		if(tc == m_controler->getChordList()->at(i).getTrackChord()) {
			nChord = i;
			break;
		}
	}

	//((EntireSong*)m_itemMap["entireSong"])->setCurrentChord(nChord);


}

void PlayerScene::loadSong(LogicalTrack* track) {
	if(m_itemMap["entireSong"] != 0)
		delete m_itemMap["entireSong"];

	((QGraphicsTextItem*)m_itemMap["songTitle"])->setHtml("<p align=\"center\">"+m_controler->getTrack()->getTrackName()+"</p>");
	((QGraphicsTextItem*)m_itemMap["songArtist"])->setHtml("<p align=\"center\">"+m_controler->getTrack()->getArtist()+"</p>");
	// Chanson entière
	m_itemMap["entireSong"] = new EntireSongBis(m_itemMap["backgnd"]);
	m_itemMap["entireSong"]->setZValue(1);
	((EntireSongBis*) m_itemMap["entireSong"])->load(track);
	connect((Controler*) parent(), SIGNAL(repaintSong()), (EntireSongBis*) m_itemMap["entireSong"], SLOT(repaintSong()) );

}