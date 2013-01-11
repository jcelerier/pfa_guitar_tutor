#ifndef EDITIONSELECTOR_H
#define EDITIONSELECTOR_H

#include <QWidget>
#include <QtGui>

#define ASSISTED_EDITOR 0
#define MANUAL_EDITOR 1

/**
 * @brief Fenêtre d'assistant de sélection d'éditeur
 */
class EditionSelector : public QWidget
{
    Q_OBJECT
public:
    explicit EditionSelector(QWidget *parent);
private slots:
    void newAssistedEditor();
    void newManualEditor();
    void setShowOption(int state);
signals:
    void newEditor(int);
    
};

#endif // EDITIONSELECTOR_H
