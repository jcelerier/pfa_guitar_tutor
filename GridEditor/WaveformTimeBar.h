#ifndef WAVEFORMTIMEBAR_H
#define WAVEFORMTIMEBAR_H

#include <QtWidgets>
#include "WaveformTimeSlider.h"
#include <Util.hpp>

/**
 * @brief Barre de temps représentant le morceau audio.
 */

enum PrecisionLevel { SAMPLE, MILLISECOND, SECOND, MINUTE } ;
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

		PrecisionLevel computePrecisionLevel();
		double computeLogPrecisionLevel();
		int getPixelSpacing();

	signals:
		void timeChanged(int, QTime);
		void somethingChanged();
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
		bool activated;

		WaveformTimeSlider* m_beginSlider, *m_barSlider, *m_endSlider, *m_playSlider;
		WaveformTimeSlider* m_clickedSlider;
		QWidget* m_parent;
		QTime m_begin, m_end;
		QGridLayout* m_layout;
		QPainter* m_painter;
		QLabel* m_container;

		QPoint m_oldMousePos, m_clickPos;


};

#endif // WAVEFORMTIMEBAR_H
