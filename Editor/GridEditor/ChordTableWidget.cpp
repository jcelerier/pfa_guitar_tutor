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
ChordTableWidget::ChordTableWidget(int column, int row)
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

    connect(m_properties, SIGNAL(triggered()), this, SLOT(showProperties()));


}

/**
 * @brief ChordTableWidget::ChordTableWidget
 * @param xgrid_file Chemin vers un fichier de sauvegarde
 *
 * Crée une grille d'accords à partir d'un fichier de sauvegarde.
 */
ChordTableWidget::ChordTableWidget(QString xgrid_file) {
	ChordTableWidget();
	open_grid(xgrid_file);
	this->setHorizontalHeaderItem(this->columnCount() - 1, new QTableWidgetItem(tr("Annotation")));
	this->setEnabled(true);

	connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(onClick(int, int)));
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
 * @brief ChordTableWidget::open_grid
 * @param xgrid_file Chemin vers un fichier de sauvegarde
 *
 * Initialise la grille d'accords courante à partir d'un fichier de sauvegarde.
 */
void ChordTableWidget::open_grid(QString xgrid_file) {
	QDomDocument doc("XGRID");
	QFile xml_doc(xgrid_file);
	if (!xml_doc.open(QIODevice::ReadOnly))
		exit(1);
	doc.setContent(&xml_doc);
	xml_doc.close();
	QDomElement chord_grid = doc.documentElement();
	this->setColumnCount(chord_grid.attribute("columns_number").toInt());
	insert_row(this->rowCount(), chord_grid.attribute("lines_number").toInt() - this->rowCount());
	this->set_name(chord_grid.attribute("name"));
	QDomNode line_node = chord_grid.firstChild();
	for (int row = 0 ; !line_node.isNull() ; line_node = line_node.nextSibling(), row ++) {
		QDomElement line = line_node.toElement();
		QDomNode case_node = line.firstChild();
		for (int column = 0 ; !case_node.nextSibling().isNull() ; case_node = case_node.nextSibling(), column ++) {
			QDomElement case_e = case_node.toElement();
			CaseItem* item = (CaseItem*) this->takeItem(row, column);
			item->set_chord(case_e.firstChild().toElement().attribute("name"));
			int r = -1, g = -1, b = -1, a = -1;
			r = case_e.lastChild().toElement().attribute("R").toInt();
			g = case_e.lastChild().toElement().attribute("G").toInt();
			b = case_e.lastChild().toElement().attribute("B").toInt();
			a = case_e.lastChild().toElement().attribute("A").toInt();
			item->set_color(r, g, b, a);
			this->setItem(row, column, item);
		}
		QTableWidgetItem* item = this->takeItem(row, columnCount() - 1);
		item->setText(line.lastChild().toElement().text());
		this->setItem(row, columnCount() - 1, item);
	}
}

/**
 * @brief ChordTableWidget::to_xml
 * @param xml_file Chemin vers un fichier de sauvegarde
 *
 * Conversion de la grille d'accords courante en format XML pour sauvegarde dans le fichier donné.
 */
void ChordTableWidget::to_xml(QString xml_file) {
	QDomDocument doc("XGRID");
	QDomElement chord_grid = doc.createElement("chord_grid");
	chord_grid.setAttribute("name", this->get_name());
	chord_grid.setAttribute("lines_number", this->rowCount());
	chord_grid.setAttribute("columns_number", this->columnCount());
	doc.appendChild(chord_grid);
	int red = -1, g = -1, b = -1, a = -1;
	for (int r = 0 ; r < this->rowCount() ; r++) {
		QDomElement line = doc.createElement("line");
		line.setAttribute("id", r);
		for (int c = 0 ; c < this->columnCount() - 1 ; c ++) {
			QDomElement case_node = doc.createElement("case");
			case_node.setAttribute("id", c);
			QDomElement chord = doc.createElement("chord");
			chord.setAttribute("name", this->item(r, c)->text());
			case_node.appendChild(chord);
			QDomElement color = doc.createElement("color");
			((CaseItem*) this->item(r, c))->get_color()->getRgb(&red, &g, &b, &a);
			color.setAttribute("G", g);
			color.setAttribute("R", red);
			color.setAttribute("A", a);
			color.setAttribute("B", b);
			case_node.appendChild(color);
			line.appendChild(case_node);
		}
		QDomElement annotation = doc.createElement("annotation");
		QDomText content = doc.createTextNode(this->item(r, this->columnCount() - 1)->text());
		annotation.appendChild(content);
		line.appendChild(annotation);
		chord_grid.appendChild(line);
	}
	QFile file(xml_file);
	file.open(QIODevice::WriteOnly);
	QTextStream ts(&file);
	ts << doc.toString();
	file.close();
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
    m_currentItem->setBeginning(t);
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

void ChordTableWidget::showProperties()
{
    m_setPartDialog->initBeginning(m_currentItem->getBeginning());
    m_setPartDialog->initPart(m_currentItem->getPart());
    m_setPartDialog->showDialogModal();
}

bool ChordTableWidget::checkBeginningTimes()
{
    QTime t;
    bool result = true;
    t = ((CaseItem*) item(0,0))->getBeginning();
    for (int r = 0 ; r < this->rowCount() ; r++) {
        for (int c = 0 ; c < this->columnCount() - 1 ; c ++) {
            if(r==0)
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

void ChordTableWidget::setTimeInfo(const QTime beginning, const QTime bar, const QTime end) { }
