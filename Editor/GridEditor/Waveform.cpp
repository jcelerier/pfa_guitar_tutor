#include "Waveform.h"
#include <QMouseEvent>
#include "SimpleMusicPlayer.h"
#include <QPixmap>
#include <cmath>
#include <QDebug>

const uint Waveform::green_color = 0xFF00FF00;
const uint Waveform::darkgreen_color = 0xFF009900;

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

	empty = true;
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


	if(!empty)
	{
		display();
	}
	else
	{
		initImage();
	}
	this->setPixmap(QPixmap::fromImage(*image));
}


void Waveform::initImage()
{
	//fond noir
	image->fill(0);

	//ligne médiane
	for(unsigned int i = 0; i < m_width; i++)
		image->setPixel(i, m_height / 2 -1, green_color);
}

/**
 * @brief QTimeToSample
 * @param t un temps au format QTime
 * @return un entier qui correspond à un nombre de samples à 44.1 khZ
 */
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

	int beg = ((SimpleMusicPlayer*) parent)->getWaveBegin();
	int end = ((SimpleMusicPlayer*) parent)->getWaveEnd();
	float size = end - beg; // nb de samples affichées

	int s_beg = QTimeToSample(l_begin);
	int s_bar = QTimeToSample(l_bar);
	int s_end = QTimeToSample(l_end);

	float tmp_begin = (((float) s_beg - (float) beg) / size);
	int pos_begin = tmp_begin * m_width;

	float tmp_end = (((float) s_end - (float) beg) / size);
	int pos_end = tmp_end * m_width;

	qDebug() << pos_end;

	//dessin du graphe
	for(unsigned int i = 0; i < m_width ; i++)
	{
		value = abs(spectrum[i] / (int) pow(2, 19 - m_height/200));
		for(unsigned int j = m_height / 2 - 1 - value; j < m_height / 2 + value; j++)
		{
			if(s_beg > beg && s_end > s_beg)
			{
				if(i <= pos_begin || i >= pos_end)
				{
					image->setPixel(i, j, darkgreen_color);
				}
				else
				{
					image->setPixel(i, j, green_color);
				}
			}
			else if(s_end > 0)
			{
				if(i >= pos_end || pos_end < 0)
				{
					image->setPixel(i, j, darkgreen_color);
				}
				else
				{
					image->setPixel(i, j, green_color);
				}
			}
			else
			{
				image->setPixel(i, j, green_color);
			}
		}
	}


	//dessin des barres

	if(s_beg > beg && s_beg < end)
	{	
		float tmp = (((float) s_beg - (float) beg) / size);
		int pos = tmp * m_width;

		for(unsigned int j = 0; j < m_height; j++)
		{
			image->setPixel(pos, j, 0xFFFFFFFFF);
		}
	}

	if(s_bar > beg && s_bar < end)
	{
		float tmp = (((float) s_bar - (float)beg) / size);
		int pos = tmp * m_width;

		for(unsigned int j = 0; j < m_height; j++)
		{
			image->setPixel(pos, j, 0xFFFFFFFFF);
		}
	}

	if(s_end > beg && s_end < end)
	{
		float tmp = (((float) s_end - (float)beg) / size);
		int pos = tmp * m_width;

		for(unsigned int j = 0; j < m_height; j++)
		{
			image->setPixel(pos, j, 0xFFFFFFFFF);
		}
	}
	//ajout des barres de temps

}

/**
 * @brief Waveform::setWidth
 * @param w Largeur en pixels
 *
 * Définit une nouvelle taille en pixels pour la waveform
 */
void Waveform::setWidth(unsigned int w)
{
	if(m_width != w)
	{
		m_width = w;
		if (spectrum != 0)
			delete spectrum;

		spectrum = new int[m_width];
	}
}

/**
 * @brief Waveform::setHeight
 * @param h Nouvelle hauteur
 */
void Waveform::setHeight(int h)
{
	m_height = h;
}

/**
 * @brief Waveform::getSpectrum
 * @return Le tableau qui contient le spectre
 */
int* Waveform::getSpectrum()
{
	return spectrum;
}

/**
 * @brief Waveform::getWidth
 * @return La largeur
 */
int Waveform::getWidth()
{
	return m_width;
}


/**
 * @brief Waveform::mouseMoveEvent
 * @param event
 *
 * Handler lors de la réception d'un mouvement à la souris
 */
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

/**
 * @brief Waveform::mousePressEvent
 * @param event
 *
 * Handler lors de la réception d'un clic à la souris
 */
void Waveform::mousePressEvent(QMouseEvent * event)
{
	clickPos = event->pos();
	oldMousePos =  event->pos();
}


/**
 * @brief Waveform::setTimers
 * @param begin
 * @param bar
 * @param end
 *
 * Slot pour la réception des timers
 */
void Waveform::setTimers(QTime begin, QTime bar, QTime end)
{
	l_begin = begin;
	l_bar = bar;
	l_end = end;

	update();
}

int Waveform::getSampleBegin()
{
	return QTimeToSample(l_begin);
}

int Waveform::getSampleBar()
{
	return QTimeToSample(l_bar);
}

int Waveform::getSampleEnd()
{
	return QTimeToSample(l_end);
}

void Waveform::activate()
{
	empty = false;
}
