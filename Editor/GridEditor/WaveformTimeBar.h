#ifndef WAVEFORMTIMEBAR_H
#define WAVEFORMTIMEBAR_H

#include <QtGui>
#include "WaveformTimeMover.h"
#include "Util.hpp"

enum PrecisionLevel { SAMPLE, MILLISECOND, SECOND, MINUTE } ;
class WaveformTimeBar : public QWidget
{
		Q_OBJECT
	public:
		explicit WaveformTimeBar(const QTime& song_end , QWidget *parent = 0);
		void draw();
		void drawText();
		void drawTimeMovers();

		PrecisionLevel computePrecisionLevel();
		double computeLogPrecisionLevel();
		int getPixelSpacing();
		void drawTextAtTime(int s_time);
		
	signals:
		
	public slots:
		void update();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		QWidget* parent;
		QTime begin, end;
		QGridLayout* layout;
		QPainter* painter;
		
};

#endif // WAVEFORMTIMEBAR_H
