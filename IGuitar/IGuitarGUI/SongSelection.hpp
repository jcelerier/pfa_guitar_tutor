#ifndef SELECTSONG_H
#define SELECTSONG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QRegExp>
#include <QGridLayout>

namespace Ui {
	class SongSelection;
}

class SongSelection : public QDialog
{
	Q_OBJECT

	public:
		explicit SongSelection(QWidget *parent = 0);
		~SongSelection();
		void init(QWidget *parent = 0);

		void showView(QGridLayout*& layout);

	private:
		Ui::SongSelection *ui;
		QTreeView * fileSystemView;
		QFileSystemModel * fileSystemModel;
		QSortFilterProxyModel *proxyModel;
		QString songName;

	public slots:
		void swapShowDialogModeless();
		void acceptData();
};

#endif // SELECTSONG_H
