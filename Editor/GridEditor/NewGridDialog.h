#ifndef NEWGRIDDIALOG_H
#define NEWGRIDDIALOG_H

#include <QDialog>

namespace Ui {
class NewGridDialog;
}

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

		/*void setArtist(QString artist);
		void setTrack(QString track);
		void setBarSize(int barsize);*/

	public slots:
		void accept();

	private:
		Ui::NewGridDialog *ui;
		QString m_trackname;
		QString m_artist;
		int m_barsize, m_lines, m_columns;
};

#endif // NEWGRIDDIALOG_H
