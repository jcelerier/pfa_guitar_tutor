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
	QRect m_windowSize;
	bool m_cntdownOver;
	int m_cntdown;
	bool m_isPlaying;
	QTimer* m_cntTimer;
	QMap <QString, QGraphicsItem*> m_itemMap;
	QSoundEffect* m_cntClick;
	QSoundEffect* m_cntClickUp;

	int  m_timeNoteSynchronized;
	int  m_lastTimeCheck;
	int  m_currentNoteDuration;
	Controler* m_controler;
	ChordDictionary *m_dictionary;
    ConfigPanel *m_configPanel;
	void disposeScene();
    TrackChord* m_lastChord;
    bool m_loaded;

};

#endif // MYSCENE_H
