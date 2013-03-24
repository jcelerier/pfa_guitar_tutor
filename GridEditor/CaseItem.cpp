#include "CaseItem.h"
#include <QtWidgets/QMenu>
#include <QDebug>
#include <QMouseEvent>
/**
 * @brief CaseItem::CaseItem
 *
 * Constructeur des cases de la grille.
 */
CaseItem::CaseItem(const bool partEditable) :  QTableWidgetItem(), m_brush(new QBrush())
{
	m_beginningTimer = QTime(0,0);
	m_part = "";
	m_brush->setColor(Qt::white);

	m_timerManuallySet = false;
	m_partEditable = partEditable;
	m_currentlyPlaying = false;

	this->setBackground(*m_brush);
	this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	this->setTextAlignment(Qt::AlignCenter);
}

/**
 * @brief CaseItem::CaseItem
 * @param item
 *
 * Constructeur surchagé (ressemble vaguement à une copie.)
 */
CaseItem::CaseItem(const CaseItem& item): QTableWidgetItem(item)
{
	this->text() = item.text();
	if(!item.m_part.isNull())
		this->m_part = item.m_part;
	this->m_beginningTimer = item.m_beginningTimer;
	this->m_brush = new QBrush(item.background());
	this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	this->setTextAlignment(Qt::AlignCenter);

}

/**
 * @brief CaseItem::~CaseItem
 *
 * Destructeur.
 */
CaseItem::~CaseItem()
{
	delete m_brush;
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
void CaseItem::set_color(int r, int g, int b, int a)
{
	m_brush->setColor(QColor(r, g, b, a));

	if(r == 255 && g == 255 && b == 255) //ugly hack, plutot faire une surcharge
	{
		QPalette c;
		this->setBackground(c.color(QPalette::Base).toRgb());
	}
	else
	{
		this->setBackgroundColor(QColor(r, g, b, a));
		//this->setBackground(*m_brush);
	}
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
	return new CaseItem((const CaseItem&) *item);
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
 * Appelé depuis isPlayingAt
 */
void CaseItem::play(bool value)
{
	m_currentlyPlaying = value;
}

/**
 * @brief CaseItem::setPlayColor
 *
 * Effectue les actions nécessaires lors de la lecture d'une case,
 * Appelé depuis itemChanged_slot (qui est automatiquement appelé lorsqu'on appelle play depuis isPlayingAt)
 */
void CaseItem::setPlayColor()
{
	set_color(20, 250, 0);
}

/**
 * @brief CaseItem::isBeingPlayed
 * @return Vrai si et seulement si la case actuelle contient l'accord qui est actuellement joué.
 */
bool CaseItem::isBeingPlayed()
{
	return m_currentlyPlaying;
}

/**
 * @brief CaseItem::restoreColor
 *
 * Remet la couleur précédente d'une case, est utilisé lors de la lecture.
 */
void CaseItem::restoreColor()
{
	set_color(255, 255, 255, 255);
}

/**
 * @brief CaseItem::setBadChordColor
 *
 * Attribue une couleur pour les cases contenant des accords non valides.
 */
void CaseItem::setBadChordColor()
{
	set_color(255, 150, 150);
}

/**
 * @brief CaseItem::setBadTimeColor
 *
 * Attribue une couleur pour les cases contenant des informations de temps non valides.
 */
void CaseItem::setBadTimeColor()
{
	set_color(255, 165, 0);
}
