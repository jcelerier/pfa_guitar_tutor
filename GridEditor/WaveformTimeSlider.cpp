#include "WaveformTimeSlider.h"
#include <Util.hpp>


/**
 * @brief WaveformTimeSlider::WaveformTimeSlider
 * @param imagepath L'image du slider à charger
 * @param type Le type (début, fin, mesure, player)
 * @param parent Parent
 *
 * Constructeur
 */
WaveformTimeSlider::WaveformTimeSlider(QString imagepath, int type, QWidget *parent):
	QWidget(parent),  type(type), m_time(0), m_pixmap(imagepath), m_pos(0)
{
}

/**
 * @brief WaveformTimeSlider::~WaveformTimeSlider
 *
 * Destructeur
 */
WaveformTimeSlider::~WaveformTimeSlider()
{

}

/**
 * @brief WaveformTimeSlider::getTime
 * @return Temps en ms
 */
int WaveformTimeSlider::getTime()
{
	return m_time;
}

/**
 * @brief WaveformTimeSlider::setTime
 * @param time Temps en ms
 */
void WaveformTimeSlider::setTime(int time)
{
	m_time = time;
}

/**
 * @brief WaveformTimeSlider::private_setTime
 * @param time Temps en ms
 *
 * Emet un signal (pour synchronisation)
 */
void WaveformTimeSlider::private_setTime(int time)
{
	m_time = time;
	emit timeChanged(type, SampleToQTime(time));
}

/**
 * @brief WaveformTimeSlider::getPos
 * @return Position en pixels
 */
int WaveformTimeSlider::getPos()
{
	return m_pos;
}

/**
 * @brief WaveformTimeSlider::setPos
 * @param pos Position en pixels
 */
void WaveformTimeSlider::setPos(int pos)
{
	m_pos = pos;
}

/**
 * @brief WaveformTimeSlider::getPixmap
 * @return Image
 */
QPixmap& WaveformTimeSlider::getPixmap()
{
	return m_pixmap;
}

/**
 * @brief WaveformTimeSlider::getPixmapSize
 * @return QRect correspondant à la taille de l'image
 */
QRect WaveformTimeSlider::getPixmapSize()
{
	return m_pixmap.rect();
}
