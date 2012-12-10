#ifndef EDITIONSELECTOR_H
#define EDITIONSELECTOR_H

#include <QWidget>
#include <QtGui>

#define ASSISTED_EDITOR 0
#define MANUAL_EDITOR 1

/**
 * @brief The EditionSelector class
 *
 * Fenêtre d'assistant de sélection d'éditeur.
 */
class EditionSelector : public QWidget
{
    Q_OBJECT
public:
    explicit EditionSelector(QWidget *parent = 0);
    
public slots:
    void newAssistedEditor();
    void newManualEditor();
signals:
    void newEditor(int);
    
};

#endif // EDITIONSELECTOR_H
