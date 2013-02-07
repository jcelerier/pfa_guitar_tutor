#ifndef EDITORPANEL_H
#define EDITORPANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "ChordTableWidget.h"
#include "AudioWindow.h"

class EditorPanel : public QWidget
{
		Q_OBJECT

    QTabWidget *tabs;
    QVBoxLayout *mainLayout;
    QWidget *pageChordTable;
    QWidget *pageAudio;
    QVBoxLayout *vboxTable;
    QVBoxLayout *vboxAudio;
    QSpacerItem* spacer;

public:
    explicit EditorPanel(ChordTableWidget* chordTable, AudioWindow *audioWindow, QWidget *parent);
    ~EditorPanel();
signals:
		
	public slots:
		
};

#endif // EDITORPANEL_H
