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

class QTimer;

class MainWindow : public QWidget{

  Q_OBJECT

 public:
  
  ~MainWindow();
  MainWindow();
  
  void playScore(bool mute);
  void stopScore();



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

  RenderArea m_renderAreas;

  bool m_mustPlay;
  bool m_mustStop;
  bool m_playMuted;

  QTimer *m_timer;
  OpenGLWidget* m_openGLWidget;
};

#endif /* MAINWINDOW_HPP */
