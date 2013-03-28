#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QtWidgets/QWidget>
#include <QImage>
#include <QPoint>
#include <QMouseEvent>
#include <QtWidgets/QLabel>
#include <QColor>
#include <QTime>
#include "MusicPlayer.h"

/**
 * @brief Affichage d'une forme d'onde
 */
class Waveform : public QLabel
{
	QWidget* m_parent;
	Q_OBJECT

	bool m_empty;

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
	int m_reductionFactor;

	QPoint m_oldMousePos;
	QPoint m_clickPos;

	QTime m_beginTime;
	QTime m_endTime;
	QTime m_barTime;


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
		void setTimer(int, QTime);
		void activate();
		void setPlayerTimer(QTime);


};

#endif // WAVEFORM_H
