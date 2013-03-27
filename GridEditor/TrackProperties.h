#ifndef TRACKPROPERTIES_H
#define TRACKPROPERTIES_H

#include <QtWidgets/QDialog>
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
		QString getComment();
		int getBarSize();
		int getTimeSignature();


		void setArtist(QString);
		void setTrack(QString);
		void setComment(QString);
		void setBarSize(unsigned int);
		void setTimeSignature(int);

	public slots:
		void accept();

	signals:
		void barsizeChanged(int);
		void timeSignatureChanged(int);
		void trackChanged();
		void artistChanged();
		void somethingChanged();

	private:
		Ui::TrackProperties *ui;

};

#endif // TRACKPROPERTIES_H
