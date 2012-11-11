#ifndef PLAYER_H
#define PLAYER_H


#include <QApplication>
#include<QPushButton>
#include<QWidget>
#include<QHBoxLayout>
#include<QSlider>
#include<iostream>
#include<QKeyEvent>
#include<QTimer>
#include<QLabel>
#include<QString>
#include<QFileInfo>
#include<QFileDialog>
#include<QStatusBar>
#include<QMenu>
#include<QMenuBar>
#include<vector>
#include<fmodex/fmod.hpp>
#include<fstream>
#include<pthread.h>


class accordTime{
public:
    int position;
    int duration;
};

class VisualisationThread;


class Player : public QWidget
{
    Q_OBJECT

private:
    QWidget* topWidget;
    QWidget* botWidget;
    QAction* exportData;
    QAction* openAction;
    FMOD::System *system;
    FMOD::Sound *sound;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *openButton;
    QPushButton *visualisation;
    QSlider *slideBar;
    QTimer *timer;
    QLabel *image;
    QLabel *led;
    bool sonEnCours;
    bool stop;
    bool paused;
    bool isInVisualisationMode;
    QLabel* compteurTemps;
    QLabel* infoMusique;
    double sec;
    double d_sec;
    double min;
    double d_min;
    QString tempsEcouleSec;
    QString tempsEcouleDSec;
    QString tempsEcouleMin;
    QString tempsEcouleDMin;
    QString musicLengthSec;
    QString musicLengthDSec;
    QString musicLengthMin;
    QString musicLengthDMin;

    QString name;

    VisualisationThread* thread;


public:
    Player();
    ~Player();

    QString nom_fichier;
    std::vector<accordTime> tempo;
    FMOD::Channel *channel;

    QPushButton* getPlayButton();
    QPushButton* getPauseButton();
    QPushButton* getOpenButton();
    QPushButton* getVisualisationButton();
    QSlider* getSlider();
    QTimer* getTimer();
    QAction* getExportAction();
    QAction* getOpenAction();
    void keyPressEvent(QKeyEvent* );
    void keyReleaseEvent(QKeyEvent *);


public slots:
    void refreshMusic(int);
    void refreshTimer();
    void initSon();
    void open();
    void pause();
    void play();
    void changePosStream(int);
    void visualiser();
    void setLedRed();
    void setLedGreen();

signals:
    void refreshMusicWhileVisualisation(int newPosition);

};


#endif // PLAYER_H
