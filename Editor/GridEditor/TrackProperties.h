#ifndef TRACKPROPERTIES_H
#define TRACKPROPERTIES_H

#include <QDialog>

namespace Ui {
class TrackProperties;
}

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

	private:
		Ui::TrackProperties *ui;
		QString m_trackname;
		QString m_artist;
		int m_barsize; //taille en cases d'une mesure, ou nombre d'accords par mesure si on préfère

};

#endif // TRACKPROPERTIES_H
