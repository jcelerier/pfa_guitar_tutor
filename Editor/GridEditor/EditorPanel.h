#ifndef EDITORPANEL_H
#define EDITORPANEL_H

#include <QWidget>
#include <QtGui>
#include "ChordTableWidget.h"
#include "AudioWindow.h"

class EditorPanel : public QWidget
{
		Q_OBJECT
	public:
		explicit EditorPanel(ChordTableWidget* chordTable, AudioWindow *audioWindow, QWidget *parent);
		
	signals:
		
	public slots:
		
};

#endif // EDITORPANEL_H
