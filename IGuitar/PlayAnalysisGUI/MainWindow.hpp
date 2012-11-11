#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QString>


#include "OpenGLWidget.h"
#include <MultiTracks.h>
#include <MusicManager.h>
#include <ScoreManager.h>

#include "RenderArea.hpp"

#define PERCENT_OF_CORRECT_NOTES_TO_GO_TO_NEXT_PART 50.0
#define CHORDS_IMAGES_LOCATION "./chordsImg"

#define INPUT_SAVE_LOCATION "./record.wav"
#define INPUT_CHORD_SAVE_LOCATION "./record.txt"

#define	FRAME_SIZE_FFT 32768

/* Avancement */
#define HOP_SIZE_FFT 8192

#define TIME_MAX_TO_RECORD 120000






class QTimer;

class MainWindow : public QWidget{

  Q_OBJECT

 public:
  
  ~MainWindow();
  MainWindow();
  
  void playRecord(bool mustPlayBack);
  void saveChords();
  void stopRecord();
  void startRecord();
  void setArgs(int argc, char* argv[]);


//private slots:
 protected:
	virtual void timeOut();

protected slots:
	virtual void timeOutSlot();
  
  
 private:
  
  void buildGUI();
  void createActions();
  void createMenus();
  void createToolBars();
  void createDockWindows();

  void initListeners();

 private:
  ScoreManager* m_scoreManager;
  ScoreManager* m_playBackScoreManager;

  MusicManager* m_musicManager;

  RenderArea m_renderAreas;

  bool m_saveChordsComputation;
  bool m_playChordsComputation;
  bool m_stopChordsComputation;
  bool m_mustBeginRecord;

  bool m_mustPlayBack;

  std::vector<std::string> m_commandLineArguments;

  QTimer *m_timer;
  OpenGLWidget* m_openGLWidget;
};

#endif /* MAINWINDOW_HPP */
