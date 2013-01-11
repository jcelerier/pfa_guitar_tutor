#include "TrackProperties.h"
#include "ui_TrackProperties.h"

TrackProperties::TrackProperties(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TrackProperties)
{
	ui->setupUi(this);
}

TrackProperties::~TrackProperties()
{
	delete ui;
}

void TrackProperties::accept()
{
	m_trackname = ui->t_trackname->text();
	m_artist = ui->t_artist->text();
	hide();
}

QString TrackProperties::getArtist()
{
	return m_artist;
}

QString TrackProperties::getTrack()
{
	return m_trackname;
}


void TrackProperties::setArtist(QString artist)
{
	m_artist = artist;
}

void TrackProperties::setTrack(QString track)
{
	m_trackname = track;
}
