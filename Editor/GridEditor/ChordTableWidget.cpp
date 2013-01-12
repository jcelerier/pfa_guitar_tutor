/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 08/05/12
*/

#include <QtXml/QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QtGui>
#include "CaseItem.h"
#include "ChordTableWidget.h"
#include "PartSetter.h"

#include "../../GuitarTutorAPI/Track/LogicalTrack.h"

/**
 * @brief ChordTableWidget::ChordTableWidget
 *
 * Initialise la grille d'accords.
 */
ChordTableWidget::ChordTableWidget() : QTableWidget(), name(new QString("")) {
	this->setEnabled(false);
}

/**
 * @brief ChordTableWidget::ChordTableWidget
 * @param column Nombre de colonnes
 * @param row Nombre de lignes
 *
 * Initialise la grille d'accords à une taille donnée.
 */
ChordTableWidget::ChordTableWidget(int column, int row, QWidget* parent)
{
	ChordTableWidget();
	this->setColumnCount(column);
	this->insert_row(0, row);
	this->setHorizontalHeaderItem(this->columnCount() - 1, new QTableWidgetItem(tr("Annotation")));
	for (int c = 0 ; c < this->columnCount() - 1 ; c ++)
		for (int r = 0 ; r < this->rowCount() ; r ++)
		{
			this->setItem(r, c, new CaseItem());
			this->setColumnWidth(c, 60);
		}
	for (int r = 0 ; r < this->rowCount() ; r ++)
	{
		this->setItem(r, this->columnCount() - 1, new QTableWidgetItem());
		this->setRowHeight(r, 40);
	}
	this->setEnabled(true);


	m_setPartDialog = new PartSetter(this);

	//menu clic droit
	m_rightClickMenu = new QMenu();

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(ShowContextMenu(const QPoint&)));

	//actions du menu
    m_properties = new QAction(tr("Properties"), this);
	m_playFromHere = new QAction(tr("Play from here"), this);

	connect(m_properties, SIGNAL(triggered()), this, SLOT(showProperties()));
	connect(m_playFromHere, SIGNAL(triggered()), this, SLOT(playFromHere()));
	connect(this, SIGNAL(play(int)), parent, SIGNAL(play(int)));

	m_currentItem = (CaseItem*) this->item(0, 0);
	setCasePart("Intro");
}

/**
 * @brief ChordTableWidget::get_name
 * @return Le nom de la grille courante
 *
 * Retourne le nom de la grille d'accords courante.
 */
QString ChordTableWidget::get_name() const {
	return *name;
}

/**
 * @brief ChordTableWidget::set_name
 * @param name Nom de la grille courante
 *
 * Attribue un nom à la grille d'accords courante.
 */
void ChordTableWidget::set_name(QString name) {
	*(this->name) = name;
}

/**
 * @brief ChordTableWidget::is_selection_empty
 * @return Vrai si et seulement si aucune case de la grille courante n'est sélectionnée
 *
 * Indique si une ou plusieurs cases de la grille d'accords sont actuellement sélectionnées.
 */
bool ChordTableWidget::is_selection_empty() {
	if (this->selectedIndexes().isEmpty())
		return true;
	return false;
}

/**
 * @brief ChordTableWidget::is_row_selected
 * @return Vrai si et seulement si une ligne entière est sélectionnée.
 *
 * Indique si une ligne de la grille d'accords est actuellement sélectionnée.
 */
bool ChordTableWidget::is_row_selected() {
	QList<QTableWidgetSelectionRange> ranges = selectedRanges();
	if (ranges.isEmpty())
		return false;
	for (QList<QTableWidgetSelectionRange>::Iterator it = ranges.begin() ; it != ranges.end() ; it ++)
		if ((*it).leftColumn() != 0 || (*it).rightColumn() != this->columnCount() - 1)
			return false;
	return true;
}

/**
 * @brief ChordTableWidget::rows_selected
 * @return La liste des lignes sélectionnées.
 *
 * Donne la liste des lignes actuellement sélectionnées dans la grille.
 */
QList<QList<int>*> ChordTableWidget::rows_selected() {
	QList<QList<int>*> range_rows;
	QList<QTableWidgetSelectionRange> ranges = selectedRanges();
	for (QList<QTableWidgetSelectionRange>::Iterator it = ranges.begin() ; it != ranges.end() ; it ++) {
		if ((*it).leftColumn() == 0 && (*it).rightColumn() == this->columnCount() - 1) {
			QList<int>* rows = new QList<int>();
			for (int r = (*it).topRow() ; r <= (*it).bottomRow() ; r ++)
				rows->push_back(r);
			range_rows.push_back(rows);
		}
	}
	return range_rows;
}

/**
 * @brief ChordTableWidget::insert_row
 * @param pos Position dans la grille des lignes à insérer
 * @param num Nombre de lignes à insérer
 *
 * Insère une ou plusieurs nouvelles lignes dans la grille à une position donnée.
 */
void ChordTableWidget::insert_row(int pos, int num) {
	for (int i = 0 ; i < num ; i ++) {
		this->insertRow(pos);
		for (int c = 0 ; c < this->columnCount() - 1 ; c ++) {
			this->setItem(pos, c, new CaseItem());
			this->setColumnWidth(c, 60);
		}
		this->setRowHeight(pos, 40);
		this->setItem(pos, this->columnCount() - 1, new QTableWidgetItem());
	}
}

/**
 * @brief ChordTableWidget::insert_row
 *
 * Insère une nouvelle ligne dans la grille.
 * @todo insérer SOIT après la case sélectionnée (si une seule case sélectionnée), SOIT à la fin du fichier
 */
void ChordTableWidget::insert_row() {
	if (is_row_selected()) {
		QList<int> rows = expand_list(rows_selected());
		for (QList<int>::Iterator it = rows.begin() ; it != rows.end() ; it ++)
			insert_row(*it);
	}
	else
		insert_row(this->rowCount());
}

/**
 * @brief ChordTableWidget::fill_selection
 * @param chord Accord
 * @param column Colonne
 *
 * Rempli la sélection courante avec l'accord donné.
 * @todo Ce code me semble bien compliqué pour ce qu'il fait...
 */
void ChordTableWidget::fill_selection(QTreeWidgetItem* chord, int column) {
	if (chord->childCount() == 0) {
		QModelIndexList index_list = this->selectedIndexes();
		for (QModelIndexList::Iterator it = index_list.begin() ; it != index_list.end() ; it ++) {
			if ((*it).column() != this->columnCount() - 1) {
				CaseItem* item = (CaseItem*) this->takeItem((*it).row(), (*it).column());
				item->set_chord(chord->text(column));
				item->set_color(0, 200, 100, 150);
				this->setItem((*it).row(), (*it).column(), item);
			}
		}
	}
}

/**
 * @brief ChordTableWidget::expand_list
 * @param list Liste
 * @return La liste modifiée
 *
 * @todo Comprendre à quoi sert cette fonction
 */
QList<int> ChordTableWidget::expand_list(QList<QList<int>*> list) {
	QList<int> result;
	for (QList<QList<int>*>::Iterator it = list.begin() ; it != list.end() ; it ++)
		result.append(**it);
	return result;
}

/**
 * @brief ChordTableWidget::copy_down_rows
 *
 * Copie les lignes actuellement sélectionnées et les insère dans la grille.
 */
void ChordTableWidget::copy_down_rows() {
	if (is_row_selected()) {
		QList<QList<int>*> ranges = rows_selected();
		int count = 0;
		QList<int>::Iterator i;
		for (QList<QList<int>*>::Iterator it = ranges.begin() ; it != ranges.end() ; it ++) {
			for (i = (**it).begin(), count = 1 ; i != (**it).end() ; i ++, count ++) {
				insert_row((**it).last() + count);
				for (int c = 0 ; c < this->columnCount() ; c ++)
					this->setItem((**it).last() + count, c, this->item(*i, c)->clone());
			}
		}
	}
}

/**
 * @brief ChordTableWidget::delete_selected_row
 *
 * Supprime les lignes dont les cases sélectionnées sont membres.
 */
void ChordTableWidget::delete_selected_row() {

	QList<QTableWidgetItem*> listItems = selectedItems();

	if(listItems.count() == 1) {
		removeRow(listItems.first()->row());
	}
	else if(listItems.count() > 1) {
		QString listRow;
		listRow.append(tr("Are you sure you want to delete lines "));

		for(QList<QTableWidgetItem*>::Iterator i = listItems.begin(); i != listItems.end() ; i++) {
			listRow.append(QString::number((**i).row()+1));
			listRow.append(" ");
		}
		listRow.append("?");

		int answer = QMessageBox::question(this, tr("Deleting lines"), listRow, QMessageBox::Yes | QMessageBox::No);

		if(answer == QMessageBox::Yes) {
			for(QList<QTableWidgetItem*>::Iterator i = listItems.begin(); i != listItems.end() ; i++) {
				if(rowCount() > 1) {
					removeRow((**i).row());
				}
			}
		}
	}
}

/**
 * @brief ChordTableWidget::delete_selected_column
 *
 * Supprime les colonnes dont les cases sélectionnées sont membres.
 */
void ChordTableWidget::delete_selected_column() {

	QList<QTableWidgetItem*> listItems = selectedItems();

	if(listItems.count() == 1) {
		removeColumn(listItems.first()->column());
	}
	else if(listItems.count() > 1) {
		QString listColumn;
		listColumn.append(tr("Are you sure you want to delete columns "));

		for(QList<QTableWidgetItem*>::Iterator i = listItems.begin(); i != listItems.end() ; i++) {
			listColumn.append(QString::number((**i).column()+1));
			listColumn.append(" ");
		}
		listColumn.append("?");

		int answer = QMessageBox::question(this, tr("Deleting columns"), listColumn, QMessageBox::Yes | QMessageBox::No);
		if(answer == QMessageBox::Yes) {

			for(QList<QTableWidgetItem*>::Iterator i = listItems.begin(); i != listItems.end() ; i++) {
				if(columnCount() > 1) {
					removeColumn((**i).column());
				}
			}
		}
	}
}

/**
 * @brief ChordTableWidget::insert_column
 *
 * Insère une nouvelle colonne dans la grille.
 * @todo insérer la colonne APRES la case sélectionnée, si une seule case sélectionnée
 */
void ChordTableWidget::insert_column() {

	insertColumn(columnCount()-1);

	for (int c = 0 ; c < this->rowCount(); c ++) {
		this->setItem(c, columnCount()-2, new CaseItem());
		this->setColumnWidth(c, 60);
	}
}


/**
 * @brief ChordTableWidget::ShowContextMenu
 * @param pos Position du curseur sur l'écran
 *
 * Affiche le menu quand on clique-droit sur une case.
 */
void ChordTableWidget::ShowContextMenu(const QPoint& pos) // this is a slot
{
    m_rightClickMenu->clear();
    QPoint globalPos = this->mapToGlobal(pos);
    m_currentItem = (CaseItem*) this->itemAt(pos);
    if (m_currentItem->column() != this->columnCount() - 1) {
        // une partie est déjà définie sur cette case
        if(m_currentItem->isPartSet())
        {
            m_rightClickMenu->addAction(m_currentItem->getPart());
            m_rightClickMenu->addSeparator();
            m_rightClickMenu->addAction(m_properties);
        }
        else
        {
            m_rightClickMenu->addAction(m_properties);
        }
		m_rightClickMenu->addAction(m_playFromHere);
        m_rightClickMenu->exec(globalPos);
    }
}

/////// TODO:
/////// Afficher le nom de partie façon GP dans les cases. (genre en petit, gras, rouge...)
/**
 * @brief ChordTableWidget::setCasePart
 * @param text Texte à mettre.
 *
 * Slot appelé lorsqu'on définit une partie.
 */
void ChordTableWidget::setCasePart(QString text)
{
	m_currentItem->setBackgroundColor(Qt::lightGray);
	m_currentItem->setPart(text);
}

void ChordTableWidget::setCaseBeginning(QTime t)
{
    m_currentItem->setBeginningTimer(t, true);
}

/**
 * @brief ChordTableWidget::removeCasePart
 * @param text Texte à mettre.
 *
 * Slot appelé lorsqu'on supprime une partie
 */
void ChordTableWidget::removeCasePart()
{
	QString text("");
	m_currentItem->setPart(text);

	m_currentItem->setBackgroundColor(Qt::white);
}

/**
 * @brief ChordTableWidget::showProperties
 *
 * Affiche les propriétés de la case.
 */
void ChordTableWidget::showProperties()
{
    m_setPartDialog->initBeginning(m_currentItem->getBeginning());
    m_setPartDialog->initPart(m_currentItem->getPart());
    m_setPartDialog->showDialogModal();
}

/**
 * @brief ChordTableWidget::checkBeginningTimes
 * @return Vrai si minutage de la case cohérent.
 *
 * Vérifie qu'il n'y a pas d'incohérences dans les temps des cases, comme
 * par exemple une case qui finirait après qu'une autre ait commencée.
 */
bool ChordTableWidget::checkBeginningTimes()
{
    QTime t;
    bool result = true;
    t = ((CaseItem*) item(0,0))->getBeginning();
    for (int r = 0 ; r < this->rowCount() ; r++) {
        for (int c = 0 ; c < this->columnCount() - 1 ; c ++) {
            if(r==0 && c == 0)
                continue;
            if(((CaseItem*) item(r,c))->getBeginning() < t) {
                ((CaseItem*) item(r,c))->setBackgroundColor(QColor(255, 165, 0));
                result = false;
            }
            else {
                if(((CaseItem*) item(r,c))->isPartSet())
                    ((CaseItem*) item(r,c))->setBackgroundColor(Qt::lightGray);
				else
                    ((CaseItem*) item(r,c))->setBackgroundColor(Qt::white);
            }
            t = ((CaseItem*) item(r,c))->getBeginning();
        }
    }
    return result;
}

/**
 * @brief ChordTableWidget::setTimeInfo
 * @param beginning Début du premier accord dans le morceau
 * @param bar Durée moyenne d'une case
 * @param end Fin du dernier accord de la grille
 *
 * Initialise les informations de temps des cases de la grille.
 */
void ChordTableWidget::setTimeInfo(const QTime beginning, const QTime bar, const QTime end)
{
	int rmax = rowCount(), cmax = columnCount();
    bool warningShown = false, modifyAllCases = false;
    for (int r = 0 ; r < rmax ; r++) {
        for (int c = 0 ; c < cmax - 1 ; c ++) {
            if(!warningShown && ((CaseItem*) item(r,c))->isTimerManuallySet()) {
                modifyAllCases = (QMessageBox::question(this, tr("Before doing stupid things"), tr("Some timers have already been set manually. Do you want to reset them too?"), QMessageBox::Yes | QMessageBox::No)) == QMessageBox::Yes;
                warningShown = true;
            }
            if(modifyAllCases || !((CaseItem*) item(r,c))->isTimerManuallySet()) {
                int ms = beginning.msec(), s = beginning.second(), m = beginning.minute(), h = beginning.hour();
                ms += bar.msec()*(r*cmax+c);
                s  += ms/1000; ms = ms%1000;
                s  += bar.second()*(r*cmax+c);
                m  += s/60; s = s%60;
                m  += bar.minute()*(r*cmax+c);
                h  += m/60; m = m%60;
                h  += bar.hour()*(r*cmax+c);
                ((CaseItem*) item(r,c))->setBeginningTimer(QTime(h,m,s,ms));
            }
        }
    }
	checkBeginningTimes();
}

/**
 * @brief TimeToMsec
 * @param t QTime à convertir
 * @return Le temps en millisecondes
 *
 * Convertit un QTime en millisecondes
 */
inline int TimeToMsec(QTime t)
{
	return t.minute() * 60000 + t.second() * 1000 + t.msec();
}

/**
 * @brief MsecToTime
 * @param t Millisecondes à convertir
 * @return QTime correspondant
 */
QTime MsecToTime(int t)
{
	int m = t / 60000;
	int s = (t - m * 60000) / 1000;
	int ms = t - m * 60000 - s * 1000;
	return QTime(0, m, s, ms);
}

LogicalTrack* ChordTableWidget::getLogicalTrack()
{
	LogicalTrack* track = new LogicalTrack();
	PartTrack* part = NULL;
	CaseItem* currentCase = NULL;
	TrackChord* currentChord = NULL;

	for(int i = 0; i < this->rowCount(); i++)
	{
		for(int j = 0; j < this->columnCount() - 1; j++) // -1 pour l'annotation
		{
			if((currentCase = (CaseItem*) this->item(i, j))->isPartSet())
			{
				// on stocke la partie précédente
				if(part != NULL)
					track->addPartTrackToList(part);

				// on crée une nouvelle partie
				part = new PartTrack(currentCase->getPart());

				//on y ajoute l'accord de la case actuelle *//* calculer s'il y a des répétitions ---------------v
				currentChord = new TrackChord(currentCase->get_chord(), TimeToMsec(currentCase->getBeginning()), 0);
				part->AddChord(currentChord);
			}
			else
			{
				currentChord = new TrackChord(currentCase->get_chord(), TimeToMsec(currentCase->getBeginning()), 0);
				part->AddChord(currentChord);
			}
		}
	}
	// on ajoute la dernière partie
	track->addPartTrackToList(part);

	return track;
}

#include <QDebug>
void ChordTableWidget::setLogicalTrack(LogicalTrack* track)
{
	CaseItem* currentCase = NULL;
	QList<PartTrack*>::const_iterator iPart;
	QList<TrackChord*>::const_iterator iChord;
	QList<PartTrack*> partList = track->getPartTrackList();
	qDebug() << partList.length(); // 36 ? wtf

	QList<TrackChord*> chordsList;

	int i=0, j=0; //ligne et colonne
	int imax = this->rowCount();
	int jmax = this->columnCount() - 2;

	qDebug() << imax << jmax << "max";

	currentCase = (CaseItem* )this->item(0, 0);
	for(iPart = partList.begin(); iPart < partList.end(); ++iPart)
	{
		qDebug() << "nouvelle partie";

		// on définit une partie sur la case actuelle
		currentCase->setPart((*iPart)->getPartName());

		// puis on remplit les accords de la partie
		chordsList = (*iPart)->getTrackChordsList();
		for(iChord = chordsList.begin(); iChord < chordsList.end() && i < imax; ++iChord)
		{
			qDebug() << i << j;

			// on stocke le nom de l'accord
			currentCase->set_chord((*iChord)->getChord());
			qDebug() << "accord" << currentCase->get_chord();

			//la durée de l'accord
			currentCase->setBeginningTimer(MsecToTime(int((*iChord)->getDuration())));
			qDebug() << "debut" << currentCase->getBeginning();

			//case suivante
			if(j < jmax)
			{
				++j;
			}
			else
			{
				j = 0;
				++i;
			}

			currentCase = (CaseItem*) this->item(i, j);
		}
	}
}

/**
 * @brief ChordTableWidget::playFromHere
 *
 * Emet un signal vers le lecteur audio pour qu'il lise à partir de la position de début.
 */
void ChordTableWidget::playFromHere()
{
	emit play(TimeToMsec(m_currentItem->getBeginning()));
}
