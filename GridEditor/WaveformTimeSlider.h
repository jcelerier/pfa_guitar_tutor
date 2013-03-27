#ifndef WAVEFORMTIMESLIDER_H
#define WAVEFORMTIMESLIDER_H

#include <QWidget>
#include <QTime>

/**
 * @brief Slider pour la barre de temps du morceau lu.
 */

class WaveformTimeSlider : public QWidget
{
		Q_OBJECT
	public:
		explicit WaveformTimeSlider(QString imagepath, int type, QWidget *parent = 0);
		~WaveformTimeSlider();
		int getTime();
		void setTime(int);
		void private_setTime(int);

		int getPos();
		void setPos(int);

		QPixmap& getPixmap();
		QRect getPixmapSize();

	signals:
        void timeChanged(int, QTime); //!< Signal émis lorsque le slider a été déplacé

	public slots:

	private:
		int m_type;
		int m_time;
		QPixmap m_pixmap;

		int m_pos;

};

#endif // WAVEFORMTIMESLIDER_H
