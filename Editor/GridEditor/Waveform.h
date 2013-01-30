#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QLabel>
#include <QColor>
#include "MusicPlayer.h"

class Waveform : public QLabel
{
	Q_OBJECT
	QImage* image;

	int* spectrum;
	unsigned int m_width;
	unsigned int m_height;

	static const uint green_color;
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
		
};

#endif // WAVEFORM_H
