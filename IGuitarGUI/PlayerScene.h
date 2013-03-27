#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>
#include <QMap>
#include <QSoundEffect>
#include <GuitarTutor/Chord.h>
#include "ButtonItem.h"
#include "MenuItem.h"
#include "ScrollingItem.h"
#include "EntireSong.h"
#include "Configuration.h"
#include "ChordDictionary.h"
#include "ConfigPanel.h"

class Controler;

/**
 * @brief Interface du player.
 *
 * Contient et gère tous les elements de l'interface.
 */
class PlayerScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit PlayerScene(QObject *parent = 0);
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	QGraphicsItem* getItem(QString);
	void updateStats(int validated, int played);
	~PlayerScene();
	Controler* getControler();
    void loadSong(LogicalTrack* track);
signals:

public slots:
	void updateScene();
	void setPlayedChord(BasicChord ch);
    void setSceneToChord(TrackChord*);
    void updateConfiguration(bool isLoopingActive, int difficulty, int continueMode);

    void play();
    void pause();
    void stop();
    void back();
	void switchMenu();
    void switchMenu(bool b);
	void switchMute();
    void switchPlay();

	void displayDictionary();
    void displayOptions();
	void playCountdown();

private:
    QRect m_windowSize;     // Taille de la scene
    bool m_cntdownOver;     // Fin du compte a rebours lors de la lecture
    int m_cntdown;          // Etat du compte a rebours
    bool m_isPlaying;       // Etat de la chanson
    QTimer* m_cntTimer;     // Timer du compte a rebours
    QMap <QString, QGraphicsItem*> m_itemMap;      // Contient tous les elements de l'interface en les associant a un nom
    QSoundEffect* m_cntClick;   // Son du metronome
    QSoundEffect* m_cntClickUp; // Son du metronome (premier)

    Controler* m_controler;     // Lien vers le controleur
	ChordDictionary *m_dictionary;
    ConfigPanel *m_configPanel;
    void disposeScene();        // Dispose tous les elements de l'interface dans la scene
    TrackChord* m_lastChord;    // Dernier accord joue
    bool m_loaded;              // La chanson a t elle ete chargee

};

#endif // MYSCENE_H
