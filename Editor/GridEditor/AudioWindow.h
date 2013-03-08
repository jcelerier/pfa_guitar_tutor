#ifndef AUDIOWINDOW_H
#define AUDIOWINDOW_H

#include <QtWidgets/QWidget>
#include <QString>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>

#include "AudioSync.h"
#include "ZoomButtons.h"
#include "SimpleMusicPlayer.h"
#include "WaveformTimeBar.h"

/**
 * @brief Tab qui contient toutes les informations relatives à l'audio
 */
class AudioWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit AudioWindow(QWidget *parent = 0);
        ~AudioWindow();
        QString getFilename();
        void setAudioFileName(QString file);
        void setWaveformData(Waveform* waveform, WaveformTimeBar* timeBar);

    public slots:
        void browseAudioFile();
        void setAudioFile();
        void resetAudioFile();
        void playFrom(int);

    private slots:
        void refreshTimerAudioSync(int);

    signals:
        void waveFullZoom();
        void waveBarZoom();
        void waveSongZoom();
        void timeSignatureChanged(int);

    private:
        QWidget* parent;
        SimpleMusicPlayer* player;
        QLabel* label;
        QLineEdit* audioFile;
        AudioSync* audioSync;
        QPushButton *browseButton;
        QGridLayout* layout;
		ZoomButtons* zoomButtons;

        Waveform* waveform;
        WaveformTimeBar* waveformTimeBar;
};

#endif // AUDIOWINDOW_H
