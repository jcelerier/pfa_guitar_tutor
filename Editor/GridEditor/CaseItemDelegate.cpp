#include "CaseItemDelegate.h"
#include <QPen>
#include <QPainter>
#include "ChordTableWidget.h"
#include "GridEditor.h"
#include "TrackProperties.h"

#include <QDebug>

CaseItemDelegate::CaseItemDelegate(QWidget *parent) :
	QStyledItemDelegate(parent), m_barsize(1)
{
	m_barPen = new QPen(QBrush(Qt::black) , 2, Qt::SolidLine, Qt::RoundCap);
	m_partFont = new QFont("Arial", 8, QFont::Bold);
}


CaseItemDelegate::~CaseItemDelegate()
{
	delete m_partFont;
	delete m_barPen;
}

void CaseItemDelegate::paint(QPainter *painter,
							 const QStyleOptionViewItem &option,
							 const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);

	QTableWidgetItem* item = ((ChordTableWidget*)parent())->itemFromIndex(index);
	if(item->column() < ((ChordTableWidget*)parent())->columnCount() -1 )
	{
		paintPartText(painter, option.rect, (CaseItem*) item);
		paintBar(painter, option.rect, (CaseItem*) item);
	}
}

void CaseItemDelegate::paintPartText(QPainter *painter, const QRect& rect, CaseItem* item) const
{
	painter->save();
	painter->setRenderHint(QPainter::TextAntialiasing, true);
	painter->setFont(*m_partFont);
	painter->setBrush(QBrush(Qt::black));

	painter->translate(rect.x(), rect.y() + (rect.height()) / 2);

	painter->drawText(QPointF(2, -9), item->getPart());
	painter->restore();

}

void CaseItemDelegate::paintBar(QPainter *painter, const QRect& rect, CaseItem* item) const
{
	painter->save();
	int itemCount;
//	int chordPerBar = ((GridEditor*) ((ChordTableWidget*) parent())->parent())->getBarSize();// = (((ChordTableWidget*) parent())->parent())->getTrackProperties();//->getBarSize(); //coucou papy

	if(item->row() == 0)
	{
		itemCount = item->column();
	}
	else if(item->row() == 1)
	{
		itemCount = item->column() + (((ChordTableWidget*) parent())->columnCount() -1);
	}
	else
	{
		itemCount = item->column() + (item->row())* (((ChordTableWidget*) parent())->columnCount() -1) ;
	}


	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(*m_barPen);
	painter->translate(rect.x(), rect.y());

	if(m_barsize == 1)
	{
		painter->drawLine(QPoint(0, 0), QPoint(0, rect.height()));
		painter->drawLine(QPoint(rect.width(), 0), QPoint(rect.width(), rect.height()));
	}
	else if(itemCount % m_barsize == 0)
	{
		painter->drawLine(QPoint(0, 0), QPoint(0, rect.height()));
	}
	else if(itemCount % m_barsize == m_barsize - 1)
	{
		painter->drawLine(QPoint(rect.width(), 0), QPoint(rect.width(), rect.height()));
	}
	painter->restore();
}

void CaseItemDelegate::setBarsize(int s)
{
	m_barsize = s;
}
