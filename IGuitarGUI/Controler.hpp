#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <QString>

#include "SongManager.h"

#include "MyView.h"
#include "PlayerChord.h"
#define MIN_SCORE_BOUND -1000
#define MAX_SCORE_BOUND 5000
#define PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART 50.0
#define CHORDS_IMAGES_LOCATION "./chordsImg"

class PlayerScene;

/**
 * @brief The Controler class
 *
 * Classe contrôleur du player.
 */
class Controler : public QObject {

	Q_OBJECT

public:
	Controler();
	~Controler();

	int elapsedTime();
	void switchPlaying();
	void pauseClock();

	bool muteState();
	void mute();
	void unmute();


	void startSong();
	void pauseSong();
	void stopSong();

	Configuration* getConfiguration();

	LogicalTrack *getTrack();
	TrackChord* getFirstChord();
    TrackChord* getCurrentChord();

public slots:
	void ticTac();
	void restartEngine();
	bool initSong();

	void currentChordSlot(TrackChord*);
    void setChordPosition(TrackChord*);
	void victoryPercent(TrackChord*, double);

signals:
	void repaintSong();
private:
	SongManager* m_songManager;

	bool m_playing, m_paused;
	QWidget * parent;
	QTimer *m_timer;
	PlayerScene *m_scene;
	MyView *m_view;

	Configuration* m_configuration;

	LogicalTrack *m_track;
	QList<PlayerChord> m_chordList;

	QTime m_globalClock;
	int m_clockOffset;

	int m_totalPlayedChords;
	int m_totalValidatedChords;

	bool m_muted;

	int well_played_chords_in_current_part;
	int played_chords_in_current_part;

	bool is_at_beginning;
};

#endif /* MAINWINDOW_HPP */

