#include "NewGridDialog.h"
#include "ui_NewGridDialog.h"

NewGridDialog::NewGridDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewGridDialog)
{
	ui->setupUi(this);
	ui->t_lines->setMinimum(1);
	ui->t_columns->setMinimum(1);
	ui->t_barsize->setMinimum(1);
}

/**
 * @brief NewGridDialog::accept
 *
 * Remplacement du mécanisme d'acceptation du dialogue.
 */
void NewGridDialog::accept()
{
	m_trackname = ui->t_trackname->text();
	m_artist = ui->t_artist->text();
	m_barsize = ui->t_barsize->value();
	m_lines = ui->t_lines->value();
	m_columns = ui->t_columns->value();

	done(QDialog::Accepted);
}


NewGridDialog::~NewGridDialog()
{
	delete ui;
}

/**
 * @brief NewGridDialog::getArtist
 * @return Artiste
 */
QString NewGridDialog::getArtist()
{
	return m_artist;
}

/**
 * @brief NewGridDialog::getTrack
 * @return Nom du morceau
 */
QString NewGridDialog::getTrack()
{
	return m_trackname;
}

/**
 * @brief NewGridDialog::getBarSize
 * @return Nombre de cases par mesure.
 */
int NewGridDialog::getBarSize()
{
	return m_barsize;
}

/**
 * @brief NewGridDialog::getLines
 * @return Nombre de lignes
 */
int NewGridDialog::getLines()
{
	return m_lines;
}

/**
 * @brief NewGridDialog::getColumns
 * @return Nombre de colonnes
 */
int NewGridDialog::getColumns()
{
	return m_columns;
}
