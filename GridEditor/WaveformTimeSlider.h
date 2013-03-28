#ifndef WAVEFORMTIMESLIDER_H
#define WAVEFORMTIMESLIDER_H

#include <QtWidgets/QWidget>
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
		/**
		 * @brief timeChanged est émis lorsque le slider a été déplacé
		 */
		void timeChanged(int, QTime);

	private:
		/**
		 * @brief m_type Type du slider : début, mesure, fin...
		 */
		int m_type;

		/**
		 * @brief m_time Position du slider en samples
		 */
		int m_time;

		/**
		 * @brief m_pixmap Image du slider
		 */
		QPixmap m_pixmap;

		/**
		 * @brief m_pos Position du slider en pixels
		 */
		int m_pos;

};

#endif // WAVEFORMTIMESLIDER_H
