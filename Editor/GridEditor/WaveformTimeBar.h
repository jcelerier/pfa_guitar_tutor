#ifndef WAVEFORMTIMEBAR_H
#define WAVEFORMTIMEBAR_H

#include <QtWidgets>
#include "WaveformTimeSlider.h"
#include "Util.hpp"

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

		void moveLeft(WaveformTimeSlider * slider);
		void moveRight(WaveformTimeSlider * slider);

		PrecisionLevel computePrecisionLevel();
		double computeLogPrecisionLevel();
		int getPixelSpacing();

	signals:
		void timeChanged(int, QTime);

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

		WaveformTimeSlider* begin_slider, *bar_slider, *end_slider, *play_slider;
		WaveformTimeSlider* clickedSlider;
		QWidget* parent;
		QTime begin, end;
		QGridLayout* layout;
		QPainter* painter;
		QLabel* container;

		QPoint oldMousePos, clickPos;


};

#endif // WAVEFORMTIMEBAR_H
