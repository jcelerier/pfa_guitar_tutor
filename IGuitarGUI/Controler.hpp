#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <QString>

#include "SongManager.h"

#include "MyView.h"
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
class Controler : public QObject
{
	Q_OBJECT

	public:
		Controler();
		~Controler();

		int elapsedTime();
		void switchPlaying();

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
		bool initSong();

        void currentChordSlot(TrackChord*);
        void updateStats(int validated, int played);

	signals:
		void repaintSong();
	private:
		SongManager* m_songManager;

        bool m_playing;
        QTimer *m_timer;        // Timer qui permet de rafraichir la scene un nombre donne de fois par secondes
        PlayerScene *m_scene;   // Scene de l'interface
        MyView *m_view;         // Vue de l'interface

		Configuration* m_configuration;

		LogicalTrack *m_track;

        bool m_muted;           // Determine si la piste est mutee

};

#endif /* MAINWINDOW_HPP */
