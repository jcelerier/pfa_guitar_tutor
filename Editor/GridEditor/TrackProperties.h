#ifndef TRACKPROPERTIES_H
#define TRACKPROPERTIES_H

#include <QDialog>
#include <QString>

namespace Ui {
class TrackProperties;
}

/**
 * @brief Onglet qui gère les propriétés (nom, artiste...) d'un morceau
 */
class TrackProperties : public QDialog
{
		Q_OBJECT
		
	public:
		explicit TrackProperties(QWidget *parent = 0);
		~TrackProperties();
		QString getArtist();
		QString getTrack();
		int getBarSize();

		void setArtist(QString artist);
		void setTrack(QString track);
		void setBarSize(int barsize);
		
	public slots:
		void accept();
	signals:
		void trackChanged();
		void artistChanged();

	private:
		Ui::TrackProperties *ui;
		QString m_trackname;
		QString m_artist;
		int m_barsize; //taille en cases d'une mesure, ou nombre d'accords par mesure si on préfère

};

#endif // TRACKPROPERTIES_H
