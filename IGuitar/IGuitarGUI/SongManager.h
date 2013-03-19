#ifndef SONGMANAGER_H
#define SONGMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <boost/thread.hpp>

#include <MusicManager.h>
#include <GuitarTutor/Chord.h>
#include <Track/LogicalTrack.h>


#define PAUSE_COMPORTMENT 0

class SongManager: public QObject

{
		Q_OBJECT
	public:
		SongManager();
		~SongManager();

		void load(LogicalTrack* track);

		void play();
		void pause();
		void stop();
		void mute(bool);

		void goToChord(TrackChord*);

		BasicChord* getPlayedInputChord();

	public slots:
		void compareChordWithPlayed();
		void checkTime();

	signals:
		void updateChord(TrackChord*);
		void lastChordCorrectness(int);


	private:
		LogicalTrack* m_track;
		MusicManager* m_musicManager;

		PartTrack* m_currentPart;
		TrackChord* m_currentChord;

		BasicChord* m_currentInputChord;

		chord_ctrl* m_chordControl;


		boost::thread *t_compareChordWithPlayed;
		boost::thread *t_checkTimer;

		QTimer m_timer;
		QTime m_time;

		int precision_in_ms;

};

#endif // SONGMANAGER_H
