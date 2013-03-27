#ifndef EDITORPANEL_H
#define EDITORPANEL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>
#include "ChordTableWidget.h"
#include "AudioWindow.h"
#include "TrackProperties.h"

/**
 * @brief Classe qui affiche les onglets
 */
class EditorPanel : public QWidget
{
		Q_OBJECT

		QTabWidget *m_tabs;
		QVBoxLayout *m_mainLayout;
		QWidget *m_pageChordTable;
		QWidget *m_pageAudio;
		QWidget *m_pageProps;
		QVBoxLayout *m_vboxTable, *m_vboxAudio, *m_vboxProps;

		QSpacerItem* m_spacer;

		ChordTableWidget* m_grid;

	public:
		explicit EditorPanel(ChordTableWidget* chordTable, AudioWindow *audioWindow, TrackProperties* trackProperties, QWidget *parent);
		void updateGrid(ChordTableWidget* chordTable);
		~EditorPanel();
	signals:

	public slots:

};

#endif // EDITORPANEL_H
