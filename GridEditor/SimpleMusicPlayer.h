#ifndef SIMPLEMUSICPLAYER_H
#define SIMPLEMUSICPLAYER_H

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QTimer>
#include <QResizeEvent>
#include <QPoint>
#include <QTime>

#include <MusicPlayer.h>

#include "Waveform.h"
#include "WaveformTimeBar.h"

#define REFRESH_DELAY 200
#define PLAYTIMER_DELAY 400

/**
 * @brief Gère l'interface graphique d'un lecteur audio simple
 */
class SimpleMusicPlayer : public QWidget
{
		Q_OBJECT

	public:
		SimpleMusicPlayer(QWidget *parent);
		~SimpleMusicPlayer();

		bool setAudioFile(QString file);

		QTime getCurrentPosition();
		QString getSong();
		int getWaveBegin();
		int getWaveEnd();

		void waveUpdate();

	public slots:
		void play();
		void pause();
		void stop();
		void refreshTimerLabel();
		void updateSlideBar();
		void changePosition(int);

		void sendTimeData();

		void waveFullZoom();
		void waveSongZoom();
		void waveBarZoom();

		void zoomIn(QPoint);
		void zoomOut(QPoint);
		void moveLeft();
		void moveRight();

	signals:
		/**
		 * @brief sigTimeData envoie la position dans la lecture du morceau
		 */
		void sigTimeData(QTime);

		/**
		 * @brief browseAudioFile permet d'ouvrir la fenêtre de choix de fichier audio
		 */
		void browseAudioFile();


	protected:
		virtual void resizeEvent(QResizeEvent * event);

	private:
		/**
		 * @brief m_parent Parent du widget
		 */
		QWidget* m_parent;

		/**
		 * @brief m_layout Disposition du widget
		 */
		QGridLayout* m_layout;

		/**
		 * @brief m_stopButton Bouton stop
		 */
		QToolButton* m_stopButton;

		/**
		 * @brief m_playButton Bouton lecture
		 */
		QToolButton* m_playButton;

		/**
		 * @brief m_playBarButton TODO bouton lecture à partir du début de la mesure
		 */
		QToolButton *m_playBarButton;

		/**
		 * @brief m_slideBar Barre avec un slider
		 */
		QSlider* m_slideBar;

		/**
		 * @brief m_timerLabel Label qui affiche le temps dans le morceau
		 */
		QLabel* m_timerLabel;

		/**
		 * @brief m_timer Sert à la mise à jour de la slidebar
		 */
		QTimer* m_timer;

		/**
		 * @brief m_playTimer Sert à la mise à jour de la slidebar de la waveform //TODO : unifier avec m_timer
		 */
		QTimer *m_playTimer;

		/**
		 * @brief m_waveform Forme d'onde qui s'affiche.
		 */
		Waveform *m_waveform;

		/**
		 * @brief m_waveformTimeBar Barre ou s'affichent les sliders de temps au dessus de la waveform
		 */
		WaveformTimeBar* m_waveformTimeBar;

		/**
		 * @brief m_player Base du player audio (dans l'API)
		 */
		MusicPlayer* m_player;

		/**
		 * @brief m_songLength Longueur du morceau (en ms)
		 */
		unsigned int m_songLength;

		/**
		 * @brief m_currentPosition Position actuelle dans le morceau (en ms)
		 */
		unsigned int m_currentPosition;

		/**
		 * @brief m_waveBegin Sample de l'extrêmité gauche de la waveform
		 */
		int m_waveBegin;

		/**
		 * @brief m_waveEnd Sample de l'extrêmité droite de la waveform
		 */
		int m_waveEnd;
};

#endif // SIMPLEMUSICPLAYER_H
