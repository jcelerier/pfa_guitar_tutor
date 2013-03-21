#include "TrackProperties.h"
#include "ui_TrackProperties.h"

#include <QDebug>

/**
 * @brief TrackProperties::TrackProperties
 * @param parent Widget parent
 */
TrackProperties::TrackProperties(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TrackProperties)
{
	m_barsize = 1;
	m_timeSignature = 4;
	ui->setupUi(this);
	connect(ui->t_timeSignature, SIGNAL(valueChanged(int)), this, SIGNAL(timeSignatureChanged(int)));
	connect(ui->t_barsize, SIGNAL(valueChanged(int)), this, SIGNAL(barsizeChanged(int)));

	accept();
}

/**
 * @brief TrackProperties::~TrackProperties
 */
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
	return ui->t_artist->text();
}

/**
 * @brief TrackProperties::getTrack
 * @return Nom du morceau
 */
QString TrackProperties::getTrack()
{
	return ui->t_trackname->text();
}

/**
 * @brief TrackProperties::getBarSize
 * @return Taille d'une mesure en cases
 */
int TrackProperties::getBarSize()
{
	return ui->t_barsize->value();
}

/**
 * @brief TrackProperties::getTimeSignature
 * @return Nombre de temps par mesure (généralement 3 ou 4)
 */
int TrackProperties::getTimeSignature()
{
    return ui->t_timeSignature->value();
}

/**
 * @brief TrackProperties::getComment
 * @return Commentaire sur le morceau
 */
QString TrackProperties::getComment()
{
	return ui->t_comment->text();
}

/** @brief TrackProperties::setComment
*
* Définit le commentaire
*/
void TrackProperties::setComment(QString t)
{
   ui->t_comment->setText(t);
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

/**
 * @brief TrackProperties::setTimeSignature
 * @param barsize Temps par mesure
 */
void TrackProperties::setTimeSignature(int time)
{
	m_timeSignature = time;
	ui->t_barsize->setValue(time);
}

//void TrackProperties::setBar(int nBar){
//    bar = nBar;
//}

//void TrackProperties::setEnd(int nEnd){
//    end = nEnd;
//}

//void TrackProperties::setBeginning(int nbeginning){
//    beginning = nbeginning;
//}

//void TrackProperties::setTimePerMesure(){
//    timePerMesure = nTimePerMesure;
//}
