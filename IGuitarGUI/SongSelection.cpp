#include "ui_SongSelection.h"
#include "SongSelection.hpp"

#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QGridLayout>

#include <QDebug>
#include "Configuration.h"

void SongSelection::showView(QGridLayout*& layout)
{
	layout->addWidget(fileSystemView, 0, 0);
}

SongSelection::SongSelection(Configuration& conf, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SongSelection),
	m_conf(conf)
{
	ui->setupUi(this);
}

SongSelection::~SongSelection()
{

}

void SongSelection::init()
{
	fileSystemModel = new QFileSystemModel;
	proxyModel = new QSortFilterProxyModel;

	QModelIndex rootModelIndex = fileSystemModel->setRootPath("Tracks/");

	proxyModel->setSourceModel(fileSystemModel);
	proxyModel->setFilterKeyColumn(2);
	proxyModel->setFilterRegExp("Folder|xml");
	ui->fileSystemView->setModel(proxyModel);

	//on n'affiche que le nom.
	ui->fileSystemView->hideColumn(1);
	ui->fileSystemView->hideColumn(2);
	ui->fileSystemView->hideColumn(3);

	// On utilise le modèle filtré pour afficher.
	ui->fileSystemView->setRootIndex(proxyModel->mapFromSource(rootModelIndex));
}

void SongSelection::swapShowDialogModeless()
{
	// Si elle est deja affichee, on cache la boite de dialogue sinon on l'affiche
	setVisible(!isVisible());
}

void SongSelection::setSongName(QModelIndex index)
{
	QModelIndex index2 = proxyModel->mapToSource(index);
	tmpSongName = fileSystemModel->filePath(index2); //filePath bug, donc il faut que les xml soient à la racine...
}

void SongSelection::acceptData()
{
	m_conf.setSongName(tmpSongName);
}
