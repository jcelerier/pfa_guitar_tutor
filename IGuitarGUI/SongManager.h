#ifndef SONGMANAGER_H
#define SONGMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer>

#include <MusicManager.h>
#include <GuitarTutor/Chord.h>
#include <Track/LogicalTrack.h>
#include "Configuration.h"

/**
 * @brief Classe qui gère le transport dans la partition.
 *
 * Contrôle la lecture audio de manière à ce que l'audio soit synchronisé avec ces informations.
 */
class SongManager: public QObject

{
		Q_OBJECT
	public:
		SongManager(QObject* parent);
		~SongManager();

		void load(LogicalTrack* track);
		void mute(bool);
        TrackChord* getCurrentChord();

        int getElapsedTime();
        void goToBeginning();
        void resetChordsFrom(TrackChord* chord);
public slots:
		void compareChordWithPlayed();
		void checkTime();
		void goToChord(TrackChord*);

		void play();
		void pause();
		void stop();

	signals:
        void currentlyPlayedChord(BasicChord); //!< Signal émis lors de l'arrivée sur un accord
        void updateChord(TrackChord*); //!< Signal émis lors du passage à l'accord suivant
        void updateStats(int, int); //!< Signal émis lorsqu'il est nécessaire de mettre à jour les statistiques de l'interface
        void nonNaturalChange(TrackChord*); //!< Signal émis lorsque l'utilisateur a demandé un déplacement dans la track
        void lastChordCorrectness(TrackChord*, double); //!< Signal émis en indiquant le pourcentage de réussite de l'accord donné


	private:
		LogicalTrack* m_track;
		MusicManager* m_musicManager;

		PartTrack* m_currentPart;
		TrackChord* m_currentChord;

		BasicChord* m_currentInputChord;
		chord_ctrl* m_chordControl;
        Configuration* m_configuration;

		QTime m_time;

        int m_numberOfValidChordChecks;   // Verification de la justesse de l'accord joue
        int m_numberOfChordChecks;

        int m_wellPlayedChordsInCurrentPart; // Verification du nombre d'accords joues dans la partie actuelle
        int m_playedChordsInCurrentPart;

        int m_totalPlayedChords;            // Nombre d'accord joué au total
        int m_totalValidatedChords;

        int m_precisionInMs;
        int m_elapsedTime;                  // Position dans le morceau, en mS
        bool m_isFirstChord;
        bool m_partRepeated;                // Vient-on de boucler sur une partie ?

};

#endif // SONGMANAGER_H
