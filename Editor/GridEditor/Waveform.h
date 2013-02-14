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
	QImage* image;

	int* spectrum;
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


	signals:
		
	public slots:
		void setTimers(QTime, QTime, QTime);
		void activate();

		
};

#endif // WAVEFORM_H
