#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QMouseEvent>
#include <QLabel>
#include <QColor>
#include <QTime>
#include "MusicPlayer.h"


class Waveform : public QLabel
{
	QWidget* parent;
	Q_OBJECT

	bool empty;

	QPixmap* m_pixmap;
	QPen* m_mainPen;
	QPen* m_darkPen;
	QPen* m_delimiterPen;
	QPen* m_playerPen;
	QPainter* m_painter;

	int* m_spectrum;
	int m_previouslyPlayedPixel;
	uint m_previouslyPlayedPixelColor;

	unsigned int m_width;
	unsigned int m_height;

	QPoint oldMousePos, clickPos;

	static const uint green_color, darkgreen_color;

	QTime l_begin, l_end, l_bar;


	protected:
		void mouseMoveEvent(QMouseEvent * event);
		void mousePressEvent(QMouseEvent * event);

	public:
		explicit Waveform(QWidget *parent, int w, int h);
		~Waveform();
		void update();
		void display();
		void initImage();
		
        void setWidth(unsigned int w);
		void setHeight(int h);

		int getWidth();
		int* getSpectrum();

		int getSampleBegin();
		int getSampleBar();
		int getSampleEnd();

		void simpleDrawColumn(int col, int value, QPen* pen);
		void drawColumn(int col, int beg, int end, int smp_begin, int smp_end, int pos_begin, int pos_end);
	signals:
		
	public slots:
		void setTimers(QTime, QTime, QTime);
		void setTimer(int, QTime);
		void activate();
		void setPlayerTimer(QTime);

		
};

#endif // WAVEFORM_H
