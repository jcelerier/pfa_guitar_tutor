#include "WaveformTimeBar.h"
#include "SimpleMusicPlayer.h"
#include <QDebug>

WaveformTimeBar::WaveformTimeBar(const QTime& song_end, QWidget *parent) :
	QWidget(parent), begin(QTime(0, 0)), end(song_end)
{
	this->parent = parent;
	painter = new QPainter();

	layout = new QGridLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(layout);

	//moche
	for(int i = 1; i < 10; ++i)
	{
		layout->setColumnMinimumWidth(i, 20);
	}

	QLabel* blbl = new QLabel(" ");
	layout->addWidget(blbl);
}

void WaveformTimeBar::draw()
{
	drawText();
	drawTimeMovers();
}

void WaveformTimeBar::drawText()
{
	const int spacing = getPixelSpacing();
	const int width = this->geometry().width();
	const int max_drawn = width / (spacing + 20);

	qDebug() << spacing;
	for(int i = 0; i < max_drawn; ++i)
	{
		painter->drawText(spacing * i,
						  0,
						  this->width(),
						  fontMetrics().height(),
						  Qt::AlignLeft,
						  "coucou");
	}

}

void WaveformTimeBar::paintEvent(QPaintEvent *event)
{
	painter->begin(this);
	painter->fillRect(event->rect(), Qt::lightGray);
	painter->setPen(Qt::black);

	drawText();
	painter->end();
}

void WaveformTimeBar::drawTimeMovers()
{

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

	qDebug() << s_begin << s_end;
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
