#include "ui_SongSelection.h"
#include "SongSelection.hpp"


#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QGridLayout>


void SongSelection::showView(QGridLayout*& layout)
{
	layout->addWidget(fileSystemView, 0, 0);
}

SongSelection::SongSelection(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SongSelection)
{
	ui->setupUi(this);
}

SongSelection::~SongSelection()
{

}

void SongSelection::acceptData()
{


}

void SongSelection::init(QWidget *parent)
{
	//fileSystemView = new QTreeView;
	fileSystemModel = new QFileSystemModel;
	proxyModel = new QSortFilterProxyModel;

	QModelIndex rootModelIndex = fileSystemModel->setRootPath("Tracks/");

	proxyModel->setSourceModel(fileSystemModel);
	proxyModel->setFilterKeyColumn(2);
	proxyModel->setFilterRegExp("Folder|xml");
	ui->fileSystemView->setModel(proxyModel);


	// Fix the TreeView on the Root Path of the Model
	ui->fileSystemView->setRootIndex(proxyModel->mapFromSource(rootModelIndex));
}

void SongSelection::swapShowDialogModeless()
{
	// Si elle est deja affichee, on cache la boite de dialogue sinon on l'affiche
	setVisible(!isVisible());
}
