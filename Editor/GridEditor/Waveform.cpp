#include "Waveform.h"
#include <QPixmap>
#include <cmath>

#include <QDebug>
const uint Waveform::green_color = 0xFF00FF00; //QColor(75, 200, 0).rgb();

Waveform::Waveform(QWidget *parent, int w, int h)
{
	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
	m_width = w;
	m_height = h;

	image = new QImage(m_width, m_height, QImage::Format_RGB32);
	spectrum = new int[m_width];

	initImage();
	this->setPixmap(QPixmap::fromImage(*image));
}


// slideBar
/**
 * @brief SimpleMusicPlayer::updateWaveform
 *
 * Actualise le graphe.
 */
void Waveform::update()
{
	if(image != 0)
		delete image;
	image = new QImage(m_width, m_height, QImage::Format_RGB32);

	display();
	this->setPixmap(QPixmap::fromImage(*image));
}

void Waveform::initImage()
{
	//fond noir
	image->fill(0);

	//ligne médiane
	for(unsigned int i = 0; i < m_width; i++)
		image->setPixel(i, m_height / 2, green_color);
}

/**
 * @brief SimpleMusicPlayer::displayGraph
 * @param waveform L'image dans laquelle on dessine
 * @param pixelWidth Longueur de l'image
 * @param pixelHeight Hauteur de l'image
 *
 * Affiche le sonogramme du morceau chargé.
 */
void Waveform::display()
{
	unsigned int value = 0;

	initImage();

	//dessin du graphe
	for(unsigned int i = 0; i < m_width ; i++)
	{
		//height = abs(table[i] / (int) pow(2, 23)); //moyenne
		value = abs(spectrum[i] / (int) pow(2, 19));//max
		for(unsigned int j = m_height / 2 - 1 - value; j < m_height / 2 + value; j++)
		{
			image->setPixel(i, j, green_color);
		}
	}
}


void Waveform::setWidth(int w)
{
	if(m_width != w)
	{
		m_width = w;
		if (spectrum != 0)
			delete spectrum;

		spectrum = new int[m_width];
	}
}

void Waveform::setHeight(int h)
{
	m_height = h;
}

int* Waveform::getSpectrum()
{
	return spectrum;
}

int Waveform::getWidth()
{
	return m_width;
}
