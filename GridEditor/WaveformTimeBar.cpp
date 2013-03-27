#include "WaveformTimeBar.h"
#include "SimpleMusicPlayer.h"
#include "AudioSync.h"
#include <QDebug>

/**
 * @brief WaveformTimeBar::WaveformTimeBar
 * @param song_end Fin du morceau
 * @param parent parent
 *
 * Constructeur
 */
WaveformTimeBar::WaveformTimeBar(const QTime& song_end, QWidget *parent) :
	QWidget(parent), m_beginTime(QTime(0, 0)), m_endTime(song_end)
{
	m_activated = false;
	m_beginSlider = new WaveformTimeSlider(":/icons/arrow_beg.png", TIMER_BEGINNING, this);
	m_barSlider = new WaveformTimeSlider(":/icons/arrow_bar.png", TIMER_BAR, this);
	m_endSlider = new WaveformTimeSlider(":/icons/arrow_end.png", TIMER_END, this);
	m_playSlider = new WaveformTimeSlider(":/icons/arrow_player.png", TIMER_PLAY, this);

	m_painter = new QPainter();
	m_layout = new QGridLayout();

	this->setBackgroundRole(QPalette::Window);
	this->setAutoFillBackground(true);

	connect(m_beginSlider, SIGNAL(timeChanged(int, QTime)), this, SIGNAL(timeChanged(int,QTime)));
	connect(m_barSlider, SIGNAL(timeChanged(int, QTime)), this, SIGNAL(timeChanged(int,QTime)));
	connect(m_endSlider, SIGNAL(timeChanged(int, QTime)), this, SIGNAL(timeChanged(int,QTime)));


	m_layout->setContentsMargins(0, 0, 0, 0);


	//moche
	for(int i = 1; i < 20; ++i)
	{
		m_layout->setColumnMinimumWidth(i, 20);
	}

	m_container = new QLabel(" ");
	m_layout->addWidget(m_container);
	this->setLayout(m_layout);

}

/**
 * @brief WaveformTimeBar::~WaveformTimeBar
 *
 * Destructeur
 */
WaveformTimeBar::~WaveformTimeBar()
{
	delete m_container;
	delete m_beginSlider;
	delete m_barSlider;
	delete m_endSlider;
	delete m_playSlider;
	delete m_painter;
	delete m_layout;
}

/**
 * @brief WaveformTimeBar::draw
 *
 * Méthode de dessin
 */
void WaveformTimeBar::draw()
{
	//drawText();
	if(m_activated)
		drawTimeSliders();
}

//**** inutilisé ****//
// on affiche, en fonction du niveau de précision :
// toutes les minutes
// 30s
// 15s
// 7s500
// 3s750
// 1s875
void WaveformTimeBar::drawText()
{
	const int spacing = getPixelSpacing();
	const int width = this->geometry().width();
	const int max_drawn = width / (spacing + 20);

	for(int i = 0; i < max_drawn; ++i)
	{
		//faire en fonction du tempo !
		drawTextAtTime(48168);
	}

}


/**
 * @brief WaveformTimeBar::paintEvent
 *
 * Méthode appelée par Qt à chaque changement.
 * Dessine.
 */
void WaveformTimeBar::paintEvent(QPaintEvent */*event*/)
{
	m_painter->begin(this);
	m_painter->setPen(Qt::black);

	draw();

	m_painter->end();
}

/**
 * @brief WaveformTimeBar::mouseReleaseEvent
 * @param event
 *
 * Appelé lorsqu'on relache le clic de la souris (pour sauver, inutilisé)
 */
void WaveformTimeBar::mouseReleaseEvent(QMouseEvent * event)
{
	QWidget::mouseReleaseEvent(event);
	emit somethingChanged();
}

/**
 * @brief WaveformTimeBar::mouseMoveEvent
 * @param event
 *
 * Déplacement de la souris
 */
void WaveformTimeBar::mouseMoveEvent(QMouseEvent * event)
{
	if(m_clickedSlider != 0)
	{
		double s_begin =  ((SimpleMusicPlayer*) parent())->getWaveBegin();
		double s_end = ((SimpleMusicPlayer*) parent())->getWaveEnd();

		float clickPercent = (float) event->x() / (float) this->width();
		float sample = clickPercent * (s_end - s_begin) + s_begin;

		m_clickedSlider->private_setTime(sample);
		if(m_clickedSlider == m_playSlider){
			QTime t = SampleToQTime(sample);
			emit(playSliderModified(TimeToMsec(t)));
		}
		update();
	}
}


/**
 * @brief WaveformTimeBar::mousePressEvent
 * @param event
 *
 * Clic
 */
void WaveformTimeBar::mousePressEvent(QMouseEvent * event)
{
	if(m_beginSlider->getPos() <= event->x() &&
	   event->x() <= m_beginSlider->getPos() + m_beginSlider->getPixmapSize().width())
	{
		m_clickedSlider = m_beginSlider;
	}
	else if(m_barSlider->getPos()  <= event->x() &&
			event->x() <= m_barSlider->getPos() + m_barSlider->getPixmapSize().width())
	{
		m_clickedSlider = m_barSlider;
	}
	else if(m_endSlider->getPos()<= event->x() &&
			event->x() <= m_endSlider->getPos() + m_endSlider->getPixmapSize().width())
	{
		m_clickedSlider = m_endSlider;
	}
	else if(m_playSlider->getPos()<= event->x() &&
			event->x() <= m_playSlider->getPos() + m_playSlider->getPixmapSize().width())
	{
	   m_clickedSlider = m_playSlider;
	}
	else
	{
		m_clickedSlider = 0;
	}
}

// **** inutilisé **** //
//algorithme général :
// on dispose du temps du pixel gauche et du pixel droit
// ainsi que du niveau de précision souhaité
// on affiche pour chaque temps qui correspond au niveau de précision et qui est entre début et fin sa valeur
// à la bonne position, avec une fonction qui prend début, fin, temps et donne la position en pixel du temps
/**
 * @brief WaveformTimeBar::drawTextAtTime
 * @param s_time Temps en sample à afficher
 *
 * Cette méthode affiche un temps donné.
 */
void WaveformTimeBar::drawTextAtTime(int s_time)
{
	double s_begin =  ((SimpleMusicPlayer*) parent())->getWaveBegin();
	double s_end = ((SimpleMusicPlayer*) parent())->getWaveEnd();

	QTime time = SampleToQTime(s_time);


	//1 : déterminer si s_time est visible :
	if(s_begin <= s_time && s_time <= s_end)
	{
		//2 : calculer sa position dans le temps (rapport entre begin et end)
		double percent = (s_time - s_begin) / (s_end - s_begin);

		//3 : obtenir la position en pixels
		double pos = this->width() * percent;

		m_painter->drawText(pos,
						  0,
						  this->width(),
						  fontMetrics().height(),
						  Qt::AlignLeft,
						  time.toString("m:ss:zzz"));

	}
}


/**
 * @brief WaveformTimeBar::drawSlider
 * @param slider Slider à dessiner
 *
 * Dessine un slider en fonction des informations dont on dispose (niveau de zoom, etc..)
 */
void WaveformTimeBar::drawSlider(WaveformTimeSlider* slider)
{
	double s_begin = 0;
	s_begin = ((SimpleMusicPlayer*) parent())->getWaveBegin();
	double s_end = 0;
	s_end = ((SimpleMusicPlayer*) parent())->getWaveEnd();
	int slider_time = slider->getTime();

	//1 : déterminer si s_time est visible :
	if(s_begin <= slider_time && slider_time <= s_end)
	{
		//2 : calculer sa position dans le temps (rapport entre begin et end)
		double percent = (slider_time - s_begin) / (s_end - s_begin);

		//3 : obtenir la position en pixels
		double pos = this->width() * percent;
		m_painter->drawPixmap(pos - slider->getPixmapSize().width() / 2,
							0,
							slider->getPixmap());

		slider->setPos(pos - slider->getPixmapSize().width() / 2);
	}
}

/**
 * @brief WaveformTimeBar::drawTimeSliders
 *
 * Dessine tous les sliders
 */
void WaveformTimeBar::drawTimeSliders()
{
	drawSlider(m_beginSlider);
	drawSlider(m_barSlider);
	drawSlider(m_endSlider);
	drawSlider(m_playSlider);
}

//TODO plutôt renvoyer le log à base 10 ?
/***** inutilisé:  à la base pensé pour calculer la précision avec laquelle afficher le temps ******/
PrecisionLevel WaveformTimeBar::computePrecisionLevel()
{
	int s_begin = QTimeToSample(m_beginTime);
	int s_end = QTimeToSample(m_endTime);
	if(s_end - s_begin < 20000)
	{
		return MILLISECOND;
	}
	else if (s_end - s_begin < 100000)
	{
		return SECOND;
	}
	else
	{
		return MINUTE;
	}
}

/****** inutilisé : à la base pensé pour calculer la précision avec laquelle afficher le temps ******/
double WaveformTimeBar::computeLogPrecisionLevel()
{
	int s_begin =  ((SimpleMusicPlayer*) parent())->getWaveBegin();
	int s_end = ((SimpleMusicPlayer*) parent())->getWaveEnd();

	if(s_end - s_begin == 0)
	{
		return -1;
	}
	else
	{
		return log(((double) s_end - (double) s_begin) / 1000) / log(10);
	}

}

//on cherche le nombre maximal d'éléments que l'on puisse mettre sachant que :
//à chaque fois qu'on atteint un nouveau palier de précision (ex : 3 -> 2)
//il faut que le spacing soit de 10 pixels entre éléments
//i.e. :
// precisionLevel = 3 -> spacing = 10
// precisionLevel = 3.5 -> spacing = 10 +width * 0.5 ?
// precisionLevel = 2 -> spacing = 10

// => fonction linéaire entre deux entiers de precisionLevel, qui va de 10 à 110 ?
// 0 -> 100
/******* inutilisé *************/
int WaveformTimeBar::getPixelSpacing()
{
	const double precisionLevel = computeLogPrecisionLevel();
	if(precisionLevel < 0)
	{
		return -1;
	}
	else
	{
		return (precisionLevel - trunc(precisionLevel)) * 100 + 10 ;
	}
}

/**
 * @brief WaveformTimeBar::update
 *
 * Pour forcer les mises à jour de l'affichage
 */
void WaveformTimeBar::update()
{
	repaint();
}


/**
 * @brief WaveformTimeBar::setTimer
 * @param type Début, Bar, End
 * @param t temps
 *
 * Sert quand on change un timer ailleurs
 */
void WaveformTimeBar::setTimer(int type, QTime t)
{
	switch(type)
	{
		case TIMER_BEGINNING:
			m_beginSlider->setTime(QTimeToSample(t));
			break;
		case TIMER_BAR:
			m_barSlider->setTime(QTimeToSample(t));
			break;
		case TIMER_END:
			m_endSlider->setTime(QTimeToSample(t));
			break;
		case TIMER_PLAY:
			m_playSlider->setTime(QTimeToSample(t));
			break;
		default:
			break;

	}
	update();

}


/**
 * @brief WaveformTimeBar::setPlayerTimer
 * @param t Temps
 *
 * Sert à changer le temps du curseur de lecture
 */
void WaveformTimeBar::setPlayerTimer(QTime t)
{
	m_playSlider->setTime(QTimeToSample(t));
	update();
}

/**
 * @brief WaveformTimeBar::activate
 *
 * Quand on charge un morceau
 */
void WaveformTimeBar::activate()
{
	m_activated = true;
}

/**
 * @brief WaveformTimeBar::deactivate
 *
 * Utilisé pour ne rien afficher quand aucun morceau n'est chargé
 */
void WaveformTimeBar::deactivate()
{
	m_activated = false;
}

