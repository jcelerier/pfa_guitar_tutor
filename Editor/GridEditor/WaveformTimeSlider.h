#ifndef WAVEFORMTIMEMOVER_H
#define WAVEFORMTIMEMOVER_H

#include <QWidget>
#include <QTime>
class WaveformTimeSlider : public QWidget
{
		Q_OBJECT
	public:
		explicit WaveformTimeSlider(QString imagepath, int type, QWidget *parent = 0);
		int getTime();
		void setTime(int);
		void private_setTime(int);

		int getPos();
		void setPos(int);

		QPixmap& getPixmap();
		QRect getPixmapSize();
	signals:
		void timeChanged(int, QTime);

	public slots:

	private:
		int type;
		int m_time;
		QPixmap m_pixmap;

		int m_pos;
		
};

#endif // WAVEFORMTIMEMOVER_H
