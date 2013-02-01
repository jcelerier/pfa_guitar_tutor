#include "Waveform.h"
#include <QMouseEvent>
#include "SimpleMusicPlayer.h"
#include <QPixmap>
#include <cmath>
#include <QDebug>

const uint Waveform::green_color = 0xFF00FF00;

Waveform::Waveform(QWidget *parent, int w, int h)
{
	this->parent = parent;
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


int QTimeToSample(QTime t)
{
	return (t.msec() + t.second() * 1000 + t.minute() * 60000) * 44.1;
	//ugly, poller la sample rate de FMOD
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
		value = abs(spectrum[i] / (int) pow(2, 19 - m_height/200));//max
		for(unsigned int j = m_height / 2 - 1 - value; j < m_height / 2 + value; j++)
		{
			image->setPixel(i, j, green_color);
		}
	}
	int beg = ((SimpleMusicPlayer*) parent)->getWaveBegin();
	int end = ((SimpleMusicPlayer*) parent)->getWaveEnd();

	int s_beg = QTimeToSample(l_begin);
	int s_bar = QTimeToSample(l_bar);
	int s_end = QTimeToSample(l_end);

//	qDebug() << beg << l_begin << l_begin.msec() << s_beg;

	if(s_beg > beg && s_beg < end)
	{
		float size = end - beg;
		float tmp = (((float)s_beg - (float)beg) / size);
		int pos = tmp * m_width;

		for(unsigned int j = 0; j < m_height; j++)
		{
			image->setPixel(pos, j, 0xFFFFFFFFF);
		}
	}

	if(s_bar > beg && s_bar < end)
	{
		float size = end - beg;
		float tmp = (((float) s_bar - (float)beg) / size);
		int pos = tmp * m_width;

		for(unsigned int j = 0; j < m_height; j++)
		{
			image->setPixel(pos, j, 0xFFFFFFFFF);
		}
	}

	if(s_end > beg && s_end < end)
	{
		float size = end - beg;
		float tmp = (((float) s_end - (float)beg) / size);
		int pos = tmp * m_width;

		for(unsigned int j = 0; j < m_height; j++)
		{
			image->setPixel(pos, j, 0xFFFFFFFFF);
		}
	}
	//ajout des barres de temps

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


void Waveform::mouseMoveEvent(QMouseEvent * event)
{
	const QPoint pos = event->pos();
	if(pos.x() < oldMousePos.x()) // on va à gauche : mouvement
	{
		((SimpleMusicPlayer*) parent)->moveLeft();
	}
	else if(pos.x() > oldMousePos.x()) // on va à droite : mouvement
	{
		((SimpleMusicPlayer*) parent)->moveRight();
	}


	if(pos.y() < oldMousePos.y()) // on va en haut : zoom
	{
		((SimpleMusicPlayer*) parent)->zoomIn(clickPos);
	}
	else if(pos.y() > oldMousePos.y()) // on va en bas : dézoom
	{
		((SimpleMusicPlayer*) parent)->zoomOut(clickPos);
	}

	oldMousePos = pos;

}

void Waveform::mousePressEvent(QMouseEvent * event)
{
	clickPos = event->pos();
	oldMousePos =  event->pos();
}

void Waveform::setTimers(QTime begin, QTime bar, QTime end)
{

	l_begin = begin;
	l_bar = bar;
	l_end = end;

	update();


}
