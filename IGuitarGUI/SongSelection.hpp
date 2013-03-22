#ifndef SELECTSONG_H
#define SELECTSONG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QtWidgets/QTreeView>
#include <QRegExp>
#include "Configuration.h"
#include <QtWidgets/QGridLayout>

namespace Ui {
	class SongSelection;
}

class SongSelection : public QDialog
{
	Q_OBJECT

	public:
		explicit SongSelection(Configuration &conf, QWidget *parent = 0);
		~SongSelection();
		void init();

		void showView(QGridLayout*& layout);


	public slots:
		void swapShowDialogModeless();
		void setSongName(QModelIndex index);
		void acceptData();


	private:
		Ui::SongSelection *ui;
		QTreeView * fileSystemView;
		QFileSystemModel * fileSystemModel;
		QSortFilterProxyModel *proxyModel;

		Configuration& m_conf;

		QString tmpSongName;

};

#endif // SELECTSONG_H