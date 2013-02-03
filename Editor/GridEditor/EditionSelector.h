#ifndef EDITIONSELECTOR_H
#define EDITIONSELECTOR_H

#include <QWidget>
#include <QtGui>

#include "NewGridDialog.h"
#define NEW_GRID 0
#define OPEN_GRID 1

/**
 * @brief Fenêtre d'assistant de sélection d'éditeur
 */
class EditionSelector : public QWidget
{
    Q_OBJECT

	QLabel *presentation;
	QTabWidget *tabs;
	QVBoxLayout *mainLayout, *vboxNew, *vboxOpen;

	QWidget *pageNew;
	QWidget *pageOpen;


public:
    explicit EditionSelector(QWidget *parent);
	~EditionSelector();
};

#endif // EDITIONSELECTOR_H
