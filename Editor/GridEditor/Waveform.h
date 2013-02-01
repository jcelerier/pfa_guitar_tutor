#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QLabel>
#include <QColor>
#include <QTime>
#include "MusicPlayer.h"



class Waveform : public QLabel
{
	QWidget* parent;
	Q_OBJECT
	QImage* image;

	int* spectrum;
	unsigned int m_width;
	unsigned int m_height;

	QPoint oldMousePos, clickPos;

	static const uint green_color;

	QTime l_begin, l_end, l_bar;


	protected:
		void mouseMoveEvent(QMouseEvent * event);
		void mousePressEvent(QMouseEvent * event);

	public:
		explicit Waveform(QWidget *parent, int w, int h);
		void update();
		void display();
		void initImage();
		
		void setWidth(int w);
		void setHeight(int h);

		int getWidth();
		int* getSpectrum();

	signals:
		
	public slots:
		void setTimers(QTime, QTime, QTime);
		
};

#endif // WAVEFORM_H
