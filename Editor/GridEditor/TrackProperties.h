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
		
	public slots:
		void accept();

	private:
		Ui::TrackProperties *ui;
		QString m_trackname;
		QString m_artist;
};

#endif // TRACKPROPERTIES_H
