#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <QString>

#include <MultiTracks.h>
#include <MusicManager.h>
#include <ScoreManager.h>

#include "PlayerScene.h"
#include "myView.h"
#include "RenderArea.hpp"
#include "playerchord.h"
#include "audioconfiguration.h"
#define PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART 50.0
#define CHORDS_IMAGES_LOCATION "./chordsImg"

class Controler : public QObject {

    Q_OBJECT

public:
    Controler();
    ~Controler();

    void playScore(bool mute);
    void stopScore();

    // amoi
    int elapsedTime();
    void startClock();
    void pauseClock();
    QList<PlayerChord> chordList;
    int test;
    LogicalTrack *track;
public slots:
    void ticTac();

protected:
    virtual void timeOut();

protected slots:
    virtual void timeOutSlot();

private:
    void initListeners();
    bool initSong();
    void startSong();
    void stopSong();

    ScoreManager* m_scoreManager;
    QList<PlayerChord> getChordList(LogicalTrack* trackName);

    bool m_mustPlay;
    bool m_mustStop;
    bool m_playing;
    QWidget * parent;
    QTimer *m_timer;
    PlayerScene *m_scene;
    myView *m_view;

    // amoi

    QTime globalClock;
    int clockOffset;



};

#endif /* MAINWINDOW_HPP */

