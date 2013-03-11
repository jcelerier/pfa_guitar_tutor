#include "CaseItem.h"
#include <QtWidgets/QMenu>
#include <QDebug>
#include <QMouseEvent>
/**
 * @brief CaseItem::CaseItem
 *
 * Constructeur des cases de la grille.
 */
CaseItem::CaseItem(const bool partEditable) :  QTableWidgetItem(), m_color(new QColor())
{
	m_beginningTimer = QTime(0,0);
	m_part = "";
	m_color->setRgb(255, 255, 255);

	m_timerManuallySet = false;
	m_partEditable = partEditable;

	this->setBackgroundColor(m_color->toRgb());
	this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	this->setTextAlignment(Qt::AlignCenter);
}

/**
 * @brief CaseItem::CaseItem
 * @param item
 *
 * Constructeur surchagé (ressemble vaguement à une copie.)
 */
CaseItem::CaseItem(const QTableWidgetItem& item) : QTableWidgetItem(item), m_color(new QColor(item.background().color()))
{
	this->text() = item.text();
}

CaseItem::~CaseItem()
{
	delete m_color;
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
	m_color->setRgb(r, g, b, a);

	if(r == 255 && g == 255 && b == 255) //ugly hack, plutot faire une surcharge
	{
		QPalette c;
		this->setBackground(c.color(QPalette::Base).toRgb());
	}
	else
	{
		this->setBackgroundColor(m_color->toRgb());
	}
}

/**
 * @brief CaseItem::get_color
 * @return La couleur actuelle de la case.
 *
 * Donne la couleur actuelle de la case. La couleur permet de déterminer l'état de la case.
 */
QColor* CaseItem::get_color() const {
	return m_color;
}

/**
 * @brief CaseItem::set_chord
 * @param chord Nom de l'accord à ajouter à la case.
 *
 * Met à jour le texte de la case pour afficher le nom de l'accord entré.
 */
void CaseItem::set_chord(QString chord)
{
	if(!BasicChord::isValidForPlayer(chord))
	{
		setBadChordColor();
	}
	else
	{
		restoreColor();
	}
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
 * question (jm): à quoi sert cette méthode si on a un constructeur par recopie ?
 */
CaseItem* CaseItem::clone() const
{
	QTableWidgetItem* item = QTableWidgetItem::clone();
	return new CaseItem((const QTableWidgetItem&) *item);
}

/**
 * @brief CaseItem::getPart
 * @return Nom de la partie
 */
QString& CaseItem::getPart()
{
	return m_part;
}

/**
 * @brief CaseItem::setPart
 * @param part Nom de la partie
 */
void CaseItem::setPart(QString part)
{
	m_part = part;
	set_color(220, 220, 255, 255);
}

/**
 * @brief CaseItem::isPartSet
 * @return Vrai si une partie est définie sur la case
 */
bool CaseItem::isPartSet()
{
	return m_part.length() > 0;
}

/**
 * @brief CaseItem::setBeginning
 * @param t Temps auquel commence l'accord
 * @param timerManuallySet Booléen qui dit si le timer a été mis par l'homme
 */
void CaseItem::setBeginning(QTime t, bool timerManuallySet)
{
	m_beginningTimer = t;
	m_timerManuallySet = timerManuallySet;
}

/**
 * @brief CaseItem::getBeginning
 * @return Temps du début de la case.
 */
QTime CaseItem::getBeginning()
{
	return m_beginningTimer;
}

/**
 * @brief CaseItem::isTimerManuallySet
 * @return Vrai si le timer a été mis par l'homme
 */
bool CaseItem::isTimerManuallySet()
{
	return m_timerManuallySet;
}

/**
 * @brief CaseItem::isPartEditable
 * @return Vrai si la partie est éditable
 */
bool CaseItem::isPartEditable()
{
	return m_partEditable;
}

/**
 * @brief CaseItem::play
 *
 * Effectue les actions nécessaires lors de la lecture d'une case,
 * en l'occurence un passage en vert.
 */
void CaseItem::play()
{
	set_color(20, 250, 0);
}

/**
 * @brief CaseItem::restoreColor
 *
 * Remet la couleur précédente d'une case, est utilisé lors de la lecture.
 */
void CaseItem::restoreColor()
{
	if(isPartSet())
	{
		set_color(220, 220, 255, 255);
	}
	else
	{
		set_color(255, 255, 255, 255);
	}
}

void CaseItem::setBadChordColor()
{
	set_color(255, 150, 150);
}

void CaseItem::setBadTimeColor()
{
	set_color(255, 165, 0);
}
