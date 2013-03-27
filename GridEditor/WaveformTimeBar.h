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


		void drawTextAtTime(int );
		void drawSlider(WaveformTimeSlider* );

		void mousePressEvent( QMouseEvent *  );
		void mouseMoveEvent(QMouseEvent * );
		void mouseReleaseEvent(QMouseEvent * );

		void moveLeft(WaveformTimeSlider * );
		void moveRight(WaveformTimeSlider * );

		PrecisionLevel computePrecisionLevel();
		double computeLogPrecisionLevel();
		int getPixelSpacing();

	signals:
		void timeChanged(int, QTime);
		void somethingChanged();
		void playSliderModified(int );

	public slots:
		void update();
		void setTimer(int , QTime );
		void setPlayerTimer(QTime );
		void activate();
		void deactivate();

	protected:
		void paintEvent(QPaintEvent *);

	private:
		bool m_activated;

		WaveformTimeSlider* m_beginSlider, *m_barSlider, *m_endSlider, *m_playSlider;
		WaveformTimeSlider* m_clickedSlider;
		QTime m_beginTime, m_endTime;
		QGridLayout* m_layout;
		QPainter* m_painter;
		QLabel* m_container;

		QPoint m_oldMousePos, m_clickPos;


};

#endif // WAVEFORMTIMEBAR_H
