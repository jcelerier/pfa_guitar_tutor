/*
Author: Fabien Fleurey
Created on 28/02/12
Last change on 14/04/12
*/

/**
 * Gestion les cases de la grille d'édition
 */

#include "CaseItem.h"

/**
 * Création initiale des cases de la grille
 */
CaseItem::CaseItem() :  QTableWidgetItem(), color(new QColor()) {
    color->setRgb(255, 255, 255);
    this->setBackgroundColor(color->toRgb());
    this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    this->setTextAlignment(Qt::AlignCenter);
}

CaseItem::CaseItem(const QTableWidgetItem& item) : QTableWidgetItem(item), color(new QColor(item.background().color()))
{}

CaseItem::~CaseItem() {
    delete color;
}

/**
 * @brief CaseItem::set_color
 * @param r Rouge
 * @param g Vert
 * @param b Bleu
 * @param a Alpha
 *
 * Configure la couleur de la case.
 */
void CaseItem::set_color(int r, int g, int b, int a = 255) {
    color->setRgb(r, g, b, a);
    this->setBackgroundColor(color->toRgb());
}

/**
 * @brief CaseItem::get_color
 * @return La couleur actuelle de la case.
 *
 * Donne la couleur actuelle de la case. La couleur permet de déterminer l'état de la case.
 */
QColor* CaseItem::get_color() const {
    return color;
}

/**
 * @brief CaseItem::set_chord
 * @param chord Nom de l'accord à ajouter à la case.
 *
 * Met à jour le texte de la case pour afficher le nom de l'accord entré.
 */
void CaseItem::set_chord(QString chord) {
    this->setText(chord);
}

/**
 * @brief CaseItem::get_chord
 * @return La chaîne de caractère correspondant à l'accord entré dans la case.
 *
 * Donne le contenu de la case. Le contenu est censé être un accord.
 */
QString CaseItem::get_chord() const {
    return this->text();
}

/**
 * @brief CaseItem::clone
 * @return Une copie de la case courante.
 *
 * Permet de créer une copie de la case sélectionnée.
 */
CaseItem* CaseItem::clone() const {
    QTableWidgetItem* item = QTableWidgetItem::clone();
    return new CaseItem((const QTableWidgetItem&) *item);
}
