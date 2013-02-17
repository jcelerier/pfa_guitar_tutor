#include "Waveform.h"
#include <QMouseEvent>
#include "SimpleMusicPlayer.h"
#include "AudioSync.h"
#include "Util.hpp"
#include <QPainter>
#include <QPixmap>
#include <cmath>
#include <QDebug>

Waveform::Waveform(QWidget *parent, int w, int h):
	QLabel(parent),
	parent(parent),
	m_previouslyPlayedPixel(0),
	m_width(w),
	m_height(h)
{
	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

	m_spectrum = new int[m_width];
	m_pixmap = new QPixmap(m_width, m_height);

	m_mainPen = new QPen(Qt::green , 2, Qt::SolidLine, Qt::RoundCap);
	m_darkPen = new QPen(QBrush(0xFF009900), 2, Qt::SolidLine, Qt::RoundCap);
	m_delimiterPen = new QPen(Qt::white , 2, Qt::SolidLine, Qt::RoundCap);
	m_playerPen = new QPen(QBrush(0xFFD8FA32) , 2, Qt::SolidLine, Qt::RoundCap);


	m_painter = new QPainter(m_pixmap);
	m_painter->setRenderHint( QPainter::HighQualityAntialiasing, true);
	m_painter->setPen(*m_mainPen);
	m_painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));


	this->setPixmap(*m_pixmap);
	empty = true;
}

Waveform::~Waveform()
{
	m_painter->end();
	delete m_pixmap;
	delete m_mainPen;
	delete m_darkPen;
	delete m_delimiterPen;
	delete m_playerPen;
	delete m_painter;
	delete m_spectrum;
}

// slideBar
/**
 * @brief SimpleMusicPlayer::updateWaveform
 *
 * Actualise le graphe.
 */
void Waveform::update()
{
	m_painter->end();
	if(m_pixmap != 0)
		delete m_pixmap;

	m_pixmap = new QPixmap(m_width, m_height);
	m_painter->begin(m_pixmap);

	m_painter->setRenderHint( QPainter::HighQualityAntialiasing, true);
	m_painter->setPen(*m_mainPen);
	m_painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));

	if(!empty)
	{
		display();
	}
	else
	{
		initImage();
	}
	this->setPixmap(*m_pixmap);
}


void Waveform::initImage()
{
	m_painter->fillRect(0, 0, (int) m_width, (int) m_height, Qt::black);
	m_painter->drawLine(0, m_height / 2 -1, m_width, m_height / 2 -1);
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
	initImage();

	int beg = ((SimpleMusicPlayer*) parent)->getWaveBegin(); //première sample affichée
	int end = ((SimpleMusicPlayer*) parent)->getWaveEnd();
	float size = end - beg; // nb de samples affichées

	int s_beg = QTimeToSample(l_begin);
	int s_bar = QTimeToSample(l_bar);
	int s_end = QTimeToSample(l_end);

	float tmp_begin = (((float) s_beg - (float) beg) / size);
	float tmp_end   = (((float) s_end - (float) beg) / size);
	float tmp_bar   = (((float) s_bar - (float) beg) / size);

	int pos_begin = tmp_begin * m_width;
	int pos_bar   = tmp_bar * m_width;
	int pos_end   = tmp_end * m_width;

	//dessin du graphe
	for( int i = 0; i < m_width ; i++)
	{
		drawColumn(i, beg, end, s_beg, s_end, pos_begin, pos_end);
	}


	//dessin des barres
	if(s_beg > beg && s_beg < end)
	{
		simpleDrawColumn(pos_begin, m_height, m_delimiterPen);
	}

	if(s_bar > beg && s_bar < end)
	{
		simpleDrawColumn(pos_bar, m_height, m_delimiterPen);
	}

	if(s_end > beg && s_end < end)
	{
		simpleDrawColumn(pos_end, m_height, m_delimiterPen);
	}
}


void Waveform::simpleDrawColumn(int col, int value, QPen* pen)
{
	m_painter->setPen(*pen);
	m_painter->drawLine(col,  m_height / 2 - 1 - value, col, m_height / 2 + value);
}

void Waveform::drawColumn(int col, int beg, int end, int smp_begin, int smp_end, int pos_begin, int pos_end)
{
	unsigned int value = std::min((unsigned int) abs(m_spectrum[col] / (int) pow(2, 19 - m_height/200)), m_height);


	if(smp_begin > beg && smp_end > smp_begin)
	{
		if(col <= pos_begin || col >= pos_end)
		{
			simpleDrawColumn(col, value, m_darkPen);
		}
		else
		{
			simpleDrawColumn(col, value, m_mainPen);
		}
	}
	else if(smp_end > 0)
	{
		if(col >= pos_end || pos_end < 0)
		{
			simpleDrawColumn(col, value, m_darkPen);
		}
		else
		{
			simpleDrawColumn(col, value, m_mainPen);
		}
	}
	else
	{
		simpleDrawColumn(col, value, m_mainPen);
	}
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
		if (m_spectrum != 0)
			delete m_spectrum;

		m_spectrum = new int[m_width];
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
	return m_spectrum;
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

void Waveform::setTimer(int type, QTime t)
{
	switch(type)
	{
		case TIMER_BEGINNING:
			l_begin = t;
			break;
		case TIMER_BAR:
			l_bar = t;
			break;
		case TIMER_END:
			l_end = t;
			break;
		default:
			break;
	}

	update();
}

void Waveform::setPlayerTimer(QTime t)
{
	int s_pos = QTimeToSample(t);

	int beg = ((SimpleMusicPlayer*) parent)->getWaveBegin();
	int end = ((SimpleMusicPlayer*) parent)->getWaveEnd();
	float size = end - beg; // nb de samples affichées

	float pos_pixel = (((float) s_pos - (float) beg) / size) * m_width;

	//vérifier si le timer est dans la zone visible
	if(beg < s_pos && s_pos < end)
	{
		update();

		simpleDrawColumn(pos_pixel, m_height, m_playerPen);

		this->setPixmap(*m_pixmap);/*
		for(unsigned int j = 0; j < m_height; j++)
		{
			m_image->setPixel(pos_pixel, j, 0xFFD8FA32);
		}

		if(pos_pixel != m_previouslyPlayedPixel)
		{
			drawColumn(m_previouslyPlayedPixel, beg, end, s_beg, s_end, pos_begin, pos_end);
			m_previouslyPlayedPixel = pos_pixel;
		}*/
	}
}
