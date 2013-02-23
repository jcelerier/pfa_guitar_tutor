#include "TrackProperties.h"
#include "ui_TrackProperties.h"

#include <QDebug>

TrackProperties::TrackProperties(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::TrackProperties)
{
	ui->setupUi(this);
    accept();
}

TrackProperties::~TrackProperties()
{
	delete ui;
}

/**
 * @brief TrackProperties::accept
 *
 * Remplacement du mécanisme d'acceptation du dialogue.
 */
void TrackProperties::accept()
{
	setTrack(ui->t_trackname->text());
	setArtist(ui->t_artist->text());
	m_barsize = ui->t_barsize->value();
}

/**
 * @brief TrackProperties::getArtist
 * @return Artiste
 */
QString TrackProperties::getArtist()
{
	return m_artist;
}

/**
 * @brief TrackProperties::getTrack
 * @return Nom du morceau
 */
QString TrackProperties::getTrack()
{
	return m_trackname;
}

/**
 * @brief TrackProperties::getBarSize
 * @return Taille d'une mesure en cases
 */
unsigned int TrackProperties::getBarSize()
{
	return m_barsize;
}

/**
 * @brief TrackProperties::setArtist
 * @param artist Artiste
 */
void TrackProperties::setArtist(QString artist)
{
	m_artist = artist;
	ui->t_artist->setText(artist);
	emit artistChanged();
}

/**
 * @brief TrackProperties::setTrack
 * @param track Nom du morceau
 */
void TrackProperties::setTrack(QString track)
{
	m_trackname = track;
	ui->t_trackname->setText(track);
	emit trackChanged();
}

/**
 * @brief TrackProperties::setBarSize
 * @param barsize Taille d'une mesure en cases
 */
void TrackProperties::setBarSize(unsigned int barsize)
{
	m_barsize = barsize;
	ui->t_barsize->setValue(barsize);
}
