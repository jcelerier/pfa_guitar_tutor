#ifndef EDITIONSELECTOR_H
#define EDITIONSELECTOR_H

#include <QWidget>
#include <QtGui>

#define NEW_GRID 0
#define OPEN_GRID 1

/**
 * @brief Fenêtre d'assistant de sélection d'éditeur
 */
class EditionSelector : public QWidget
{
    Q_OBJECT
public:
    explicit EditionSelector(QWidget *parent);
};

#endif // EDITIONSELECTOR_H
