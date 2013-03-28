#ifndef NEWGRIDDIALOG_H
#define NEWGRIDDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui {
class NewGridDialog;
}

/**
 * @brief Fenêtre de création de nouveau morceau
 */
class NewGridDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit NewGridDialog(QWidget *parent = 0);
		~NewGridDialog();
		QString getArtist();
		QString getTrack();
		int getBarSize();

		int getLines();
		int getColumns();

	public slots:
		void accept();

	private:
		/**
		 * @brief ui La vue de cette classe
		 */
		Ui::NewGridDialog *ui;

		/**
		 * @brief m_trackname Nom du morceau
		 */
		QString m_trackname;

		/**
		 * @brief m_artist Artiste
		 */
		QString m_artist;

		/**
		 * @brief m_barsize Taille de la mesure
		 */
		int m_barsize;

		/**
		 * @brief m_lines Nombre de lignes souhaitées
		 */
		int m_lines;

		/**
		 * @brief m_columns Nombre de colonnes souhaitées
		 */
		int m_columns;
};

#endif // NEWGRIDDIALOG_H
