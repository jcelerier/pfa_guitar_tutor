#ifndef AUDIOWINDOW_H
#define AUDIOWINDOW_H

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
		void setAudioFileName(QString);
		void setWaveformData(Waveform* , WaveformTimeBar* );
		void setBar(const int );
		void setBeginning(const int );
		void setEnd(const int );
		int getBar();
		int getBeginning();
		int getEnd();


	public slots:
		void browseAudioFile();
		void setAudioFile();
		void playFrom(int);

	private slots:
		void refreshTimerAudioSync(int);

	signals:
		/**
		 * @brief somethingChanged est émis lorsqu'une donnée quelconque change. Utilisé (théoriquement) pour la sauvegarde.
		 */
		void somethingChanged();

		/**
		 * @brief waveFullZoom est envoyé à la waveform pour signifier qu'on veut un zoom sur tout le fichier audio
		 */
		void waveFullZoom();

		/**
		 * @brief waveBarZoom est envoyé à la waveform pour signifier qu'on veut un zoom sur la première mesure
		 */
		void waveBarZoom();

		/**
		 * @brief waveSongZoom est envoyé à la waveform pour signifier qu'on veut un zoom sur le morceau tel que défini par les mesures
		 */
		void waveSongZoom();

		/**
		 * @brief timeSignatureChanged signal pour dire que la signature temporelle a changé
		 */
		void timeSignatureChanged(int);

	private:
		/**
		 * @brief m_player Le player audio
		 */
		SimpleMusicPlayer* m_player;

		/**
		 * @brief m_audioFile L'endroit ou s'affiche le nom du fichier audio choisi
		 */
		QLineEdit* m_audioFile;

		/**
		 * @brief m_audioSync Le sous-panneau de synchronisation entre l'audio et la grille (partie supérieure gauche)
		 */
		AudioSync* m_audioSync;

		/**
		 * @brief m_browseButton permet de choisir un fichier audio
		 */
		QPushButton *m_browseButton;

		/**
		 * @brief m_layout layout du widget
		 */
		QGridLayout* m_layout;

		/**
		 * @brief m_label Affiche "Fichier audio"
		 */
		QLabel* m_label;

		/**
		 * @brief m_zoomButtons les boutons de zoom sous la waveform
		 */
		ZoomButtons* m_zoomButtons;

		/**
		 * @brief m_waveform la waveform
		 */
		Waveform* m_waveform;

		/**
		 * @brief m_waveformTimeBar la barre ou se trouvent les sliders au dessus de la waveform
		 */
		WaveformTimeBar* m_waveformTimeBar;
};

#endif // AUDIOWINDOW_H
