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
		/**
		 * @brief barsizeChanged est émis quand le nombre d'accords par mesure change
		 */
		void barsizeChanged(int);

		/**
		 * @brief timeSignatureChanged est émis quand la signature temporelle change
		 */
		void timeSignatureChanged(int);

		/**
		 * @brief trackChanged est émis quand le nom du morceau change
		 */
		void trackChanged();

		/**
		 * @brief artistChanged est émis quand l'artiste change
		 */
		void artistChanged();

		/**
		 * @brief somethingChanged est émis dès que quelquechose change //TODO (pour sauvegarde)
		 */
		void somethingChanged();

	private:
		/**
		 * @brief ui Vue correspondant à la classe
		 */
		Ui::TrackProperties *ui;

};

#endif // TRACKPROPERTIES_H
