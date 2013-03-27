#ifndef EDITIONSELECTOR_H
#define EDITIONSELECTOR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include "NewGridDialog.h"

#define NEW_GRID 0
#define OPEN_GRID 1

/**
 * @brief Fenêtre d'assistant de sélection d'éditeur
 */
class EditionSelector : public QWidget
{
	Q_OBJECT

	QLabel *m_presentation;
	QTabWidget *m_tabs;
	QVBoxLayout *m_mainLayout, *m_vboxNew, *m_vboxOpen;

	QWidget *m_pageNew;
	QWidget *m_pageOpen;


public:
	explicit EditionSelector(QWidget *parent);
	~EditionSelector();
};

#endif // EDITIONSELECTOR_H
