#include "WaveformTimeBar.h"
#include "SimpleMusicPlayer.h"
#include "AudioSync.h"
#include <QDebug>


WaveformTimeBar::WaveformTimeBar(const QTime& song_end, QWidget *parent) :
	QWidget(parent), begin(QTime(0, 0)), end(song_end)
{
	begin_slider = new WaveformTimeSlider(":/icons/arrow_beg.png", TIMER_BEGINNING, this);
	bar_slider = new WaveformTimeSlider(":/icons/arrow_bar.png", TIMER_BAR, this);
	end_slider = new WaveformTimeSlider(":/icons/arrow_end.png", TIMER_END, this);

	play_slider = new WaveformTimeSlider(":/icons/arrow_player.png", TIMER_PLAY, this);


	connect(begin_slider, SIGNAL(timeChanged(int, QTime)), this, SIGNAL(timeChanged(int,QTime)));
	connect(bar_slider, SIGNAL(timeChanged(int, QTime)), this, SIGNAL(timeChanged(int,QTime)));
	connect(end_slider, SIGNAL(timeChanged(int, QTime)), this, SIGNAL(timeChanged(int,QTime)));

	this->parent = parent;
	this->setBackgroundRole(QPalette::Window);
	this->setAutoFillBackground(true);

	painter = new QPainter();

	layout = new QGridLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(layout);

	//moche
	for(int i = 1; i < 20; ++i)
	{
		layout->setColumnMinimumWidth(i, 20);
	}

	QLabel* blbl = new QLabel(" ");
	layout->addWidget(blbl);
}

void WaveformTimeBar::draw()
{
	//drawText();
	drawTimeSliders();
}

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
void WaveformTimeBar::paintEvent(QPaintEvent *event)
{
	painter->begin(this);
	painter->setPen(Qt::black);

	draw();

	painter->end();
}


void WaveformTimeBar::mouseMoveEvent(QMouseEvent * event)
{
	if(clickedSlider != 0)
	{
		double s_begin =  ((SimpleMusicPlayer*) parent)->getWaveBegin();
		double s_end = ((SimpleMusicPlayer*) parent)->getWaveEnd();

		float clickPercent = (float) event->x() / (float) this->width();
		float sample = clickPercent * (s_end - s_begin) + s_begin;

		clickedSlider->private_setTime(sample);
		update();
	}
}

void WaveformTimeBar::mousePressEvent(QMouseEvent * event)
{
	if(begin_slider->getPos() <= event->x() &&
	   event->x() <= begin_slider->getPos() + begin_slider->getPixmapSize().width())
	{
		clickedSlider = begin_slider;
	}
	else if(bar_slider->getPos()  <= event->x() &&
			event->x() <= bar_slider->getPos() + bar_slider->getPixmapSize().width())
	{
		clickedSlider = bar_slider;
	}
	else if(end_slider->getPos()<= event->x() &&
			event->x() <= end_slider->getPos() + end_slider->getPixmapSize().width())
	{
		clickedSlider = end_slider;
	}
	else
	{
		clickedSlider = 0;
	}
}


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
	double s_begin =  ((SimpleMusicPlayer*) parent)->getWaveBegin();
	double s_end = ((SimpleMusicPlayer*) parent)->getWaveEnd();

	QTime time = SampleToQTime(s_time);


	//1 : déterminer si s_time est visible :
	if(s_begin <= s_time && s_time <= s_end)
	{
		//2 : calculer sa position dans le temps (rapport entre begin et end)
		double percent = (s_time - s_begin) / (s_end - s_begin);

		//3 : obtenir la position en pixels
		double pos = this->width() * percent;

		painter->drawText(pos,
						  0,
						  this->width(),
						  fontMetrics().height(),
						  Qt::AlignLeft,
						  time.toString("m:ss:zzz"));

	}
}

void WaveformTimeBar::drawSlider(WaveformTimeSlider* slider)
{
	double s_begin =  ((SimpleMusicPlayer*) parent)->getWaveBegin();
	double s_end = ((SimpleMusicPlayer*) parent)->getWaveEnd();



	//1 : déterminer si s_time est visible :
	if(s_begin <= slider->getTime() && slider->getTime() <= s_end)
	{
		//2 : calculer sa position dans le temps (rapport entre begin et end)
		double percent = (slider->getTime() - s_begin) / (s_end - s_begin);

		//3 : obtenir la position en pixels
		double pos = this->width() * percent;

		painter->drawPixmap(pos - slider->getPixmapSize().width() / 2,
							0,
							slider->getPixmap());

		slider->setPos(pos - slider->getPixmapSize().width() / 2);
	}
}

void WaveformTimeBar::drawTimeSliders()
{
	drawSlider(begin_slider);
	drawSlider(bar_slider);
	drawSlider(end_slider);
	drawSlider(play_slider);
}

//TODO plutôt renvoyer le log à base 10 ?
PrecisionLevel WaveformTimeBar::computePrecisionLevel()
{
	int s_begin = QTimeToSample(begin);
	int s_end = QTimeToSample(end);
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


double WaveformTimeBar::computeLogPrecisionLevel()
{
	int s_begin =  ((SimpleMusicPlayer*) parent)->getWaveBegin();
	int s_end = ((SimpleMusicPlayer*) parent)->getWaveEnd();

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

void WaveformTimeBar::update()
{
	repaint();
}

void WaveformTimeBar::setTimer(int type, QTime t)
{
	switch(type)
	{
		case TIMER_BEGINNING:
			begin_slider->setTime(QTimeToSample(t));
			break;
		case TIMER_BAR:
			bar_slider->setTime(QTimeToSample(t));
			break;
		case TIMER_END:
			end_slider->setTime(QTimeToSample(t));
			break;
		default:
			break;
	}
	update();
}

void WaveformTimeBar::setPlayerTimer(QTime t)
{
	play_slider->setTime(QTimeToSample(t));
	update();
}
