#ifndef WAVEFORMTIMEBAR_H
#define WAVEFORMTIMEBAR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QGridLayout>
#include <Util.hpp>
#include "WaveformTimeSlider.h"


/**
 * @brief Barre de temps représentant le morceau audio.
 */

//enum PrecisionLevel { SAMPLE, MILLISECOND, SECOND, MINUTE } ;
class WaveformTimeBar : public QWidget
{
		Q_OBJECT
	public:
		explicit WaveformTimeBar(const QTime& song_end , QWidget *parent = 0);

		~WaveformTimeBar();
		void draw();
		void drawText();
		void drawTimeSliders();


		void drawTextAtTime(int s_time);
		void drawSlider(WaveformTimeSlider* slider);

		void mousePressEvent( QMouseEvent * event );
		void mouseMoveEvent(QMouseEvent * event);
		void mouseReleaseEvent(QMouseEvent * event);

		void moveLeft(WaveformTimeSlider * slider);
		void moveRight(WaveformTimeSlider * slider);

		//PrecisionLevel computePrecisionLevel();
		//double computeLogPrecisionLevel();
		//int getPixelSpacing();

	signals:
		/**
		 * @brief timeChanged est émis lorsqu'on bouge un slider
		 */
		void timeChanged(int, QTime);

		/**
		 * @brief somethingChanged  //TODO est utilisé pour la sauvegarde
		 */
		void somethingChanged();

		/**
		 * @brief playSliderModified est utilisé lorsqu'on bouge le slider de lecture
		 */
		void playSliderModified(int position);

	public slots:
		void update();
		void setTimer(int type, QTime t);
		void setPlayerTimer(QTime t);
		void activate();
		void deactivate();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		/**
		 * @brief activated Vrai si un morceau est chargé
		 */
		bool activated;

		/**
		 * @brief m_beginSlider Slider du début
		 */
		WaveformTimeSlider *m_beginSlider;

		/**
		 * @brief m_barSlider Slider de mesure
		 */
		WaveformTimeSlider *m_barSlider;

		/**
		 * @brief m_endSlider Slider de fin
		 */
		WaveformTimeSlider *m_endSlider;

		/**
		 * @brief m_playSlider Slider de lecture
		 */
		WaveformTimeSlider *m_playSlider;

		/**
		 * @brief m_clickedSlider Slider cliqué par la souris
		 */
		WaveformTimeSlider* m_clickedSlider;

		/**
		 * @brief m_parent Parent du widget
		 */
		QWidget* m_parent;

		/**
		 * @brief m_begin Début du morceau
		 */
		QTime m_begin;

		/**
		 * @brief m_end Fin du morceau
		 */
		QTime m_end;

		/**
		 * @brief m_layout Disposition du widget
		 */
		QGridLayout* m_layout;

		/**
		 * @brief m_painter Sert à peindre le widget
		 */
		QPainter* m_painter;

		/**
		 * @brief m_container Nécessaire pour afficher le widget
		 */
		QLabel* m_container;

		/**
		 * @brief m_oldMousePos Ancienne position de la souris
		 */
		QPoint m_oldMousePos;

		/**
		 * @brief m_clickPos Position actuelle de la souris
		 */
		QPoint m_clickPos;
};

#endif // WAVEFORMTIMEBAR_H
