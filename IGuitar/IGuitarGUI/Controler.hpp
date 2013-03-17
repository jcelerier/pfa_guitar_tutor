#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <QString>

#include <MultiTracks.h>
#include <MusicManager.h>
#include <ScoreManager.h>

#include "MyView.h"
#include "PlayerChord.h"
#include "PlayerScene.h"
#define MIN_SCORE_BOUND -1000
#define MAX_SCORE_BOUND 5000
#define PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART 50.0
#define CHORDS_IMAGES_LOCATION "./chordsImg"

class Controler : public QObject {

	Q_OBJECT

public:
	Controler();
	~Controler();

	int elapsedTime();
	void startClock();
	void pauseClock();
    QList<PlayerChord>* getChordList();
	QList<PlayerChord> getChordList(LogicalTrack* trackName);
    LogicalTrack *getTrack();
public slots:
	void ticTac();
	void restartEngine();

private:
	bool initSong();
	void startSong();
	void stopSong();

	ScoreManager* m_scoreManager;
	MusicManager* m_musicManager;

	bool m_playing;
	QWidget * parent;
    QTimer *m_timer;
	PlayerScene *m_scene;
    MyView *m_view;

    Configuration* m_configuration;

    LogicalTrack *m_track;
    QList<PlayerChord> chordList;

	std::string m_currentPart;
	QTime globalClock;
	int clockOffset;

};

#endif /* MAINWINDOW_HPP */

