#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QString>

#include "../../GuitarTutorAPI/MultiTracks.h"
#include "../../GuitarTutorAPI/MusicManager.h"
#include "../../GuitarTutorAPI/ScoreManager.h"

#include "RenderArea.hpp"
#include "audioconfiguration.h"
#define PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART 50.0
#define CHORDS_IMAGES_LOCATION "./chordsImg"

class QTimer;

class MainWidget : public QWidget {

		Q_OBJECT

	public:

		~MainWidget();
		MainWidget();

		void playScore(bool mute);
		void stopScore();


	protected:
		virtual void timeOut();

	protected slots:
		virtual void timeOutSlot();

	private:
		void buildGUI();
		void createActions();
		void initListeners();

		ScoreManager* m_scoreManager;
		RenderArea m_renderAreas;

		bool m_mustPlay;
		bool m_mustStop;
		bool m_playMuted;

		QTimer *m_timer;
};

#endif /* MAINWINDOW_HPP */

