#ifndef EDITORPANEL_H
#define EDITORPANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "ChordTableWidget.h"
#include "AudioWindow.h"
#include "TrackProperties.h"

/**
 * @brief Classe qui affiche les onglets
 */
class EditorPanel : public QWidget
{
		Q_OBJECT

    QTabWidget *tabs;
    QVBoxLayout *mainLayout;
    QWidget *pageChordTable;
    QWidget *pageAudio;
	QWidget *pageProps;
	QVBoxLayout *vboxTable, *vboxAudio, *vboxProps;

    QSpacerItem* spacer;

	ChordTableWidget* grid;

public:
	explicit EditorPanel(ChordTableWidget* chordTable, AudioWindow *audioWindow, TrackProperties* trackProperties, QWidget *parent);
	void updateGrid(ChordTableWidget* chordTable);

signals:
		
	public slots:
		
};

#endif // EDITORPANEL_H
