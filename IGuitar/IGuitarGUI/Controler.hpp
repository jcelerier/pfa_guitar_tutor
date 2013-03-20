#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <QString>

#include <MultiTracks.h>
#include <MusicManager.h>
#include <ScoreManager.h>

#include "SongManager.h"

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
	void switchPlaying();
	void pauseClock();
	QList<PlayerChord>* getChordList();
	QList<PlayerChord> getChordList(LogicalTrack* trackName);
	LogicalTrack *getTrack();

public slots:
	void ticTac();
	void restartEngine();

	void currentChordSlot(TrackChord*);
	void victoryPercent(double);


private:
	bool initSong();
	void startSong();
	void pauseSong();

	ScoreManager* m_scoreManager;
	SongManager* m_songManager;
	MusicManager* m_musicManager;

	bool m_playing, m_paused;
	QWidget * parent;
	QTimer *m_timer;
	PlayerScene *m_scene;
	MyView *m_view;

	Configuration* m_configuration;

	LogicalTrack *m_track;
	QList<PlayerChord> m_chordList;

	std::string m_currentPart;
	QTime m_globalClock;
	int m_clockOffset;
	int m_savedClock;

};

#endif /* MAINWINDOW_HPP */

