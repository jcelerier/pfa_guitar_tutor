#ifndef EDITORPANEL_H
#define EDITORPANEL_H


#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>
#include "ChordTableWidget.h"
#include "AudioWindow.h"
#include "TrackProperties.h"

/**
 * @brief Classe qui affiche les onglets de la fenêtre principale
 */
class EditorPanel : public QWidget
{
		Q_OBJECT

	public:
		explicit EditorPanel(ChordTableWidget* , AudioWindow *, TrackProperties* , QWidget *);
		void updateGrid(ChordTableWidget* );
		~EditorPanel();

	private:
		/**
		 * @brief m_tabs Pages de tabulation
		 */
		QTabWidget *m_tabs;

		/**
		 * @brief m_mainLayout Layout ou tout est disposé
		 */
		QVBoxLayout *m_mainLayout;

		/**
		 * @brief m_vboxTable Layout pour la grille d'accords
		 */
		QVBoxLayout *m_vboxTable;

		/**
		 * @brief m_vboxAudio Layout pour la page de configuration audio
		 */
		QVBoxLayout *m_vboxAudio;

		/**
		 * @brief m_vboxProps Layout pour la page de propriétés
		 */
		QVBoxLayout *m_vboxProps;

		/**
		 * @brief m_pageChordTable Widget  pour la grille d'accords
		 */
		QWidget *m_pageChordTable;

		/**
		 * @brief m_pageAudio Widget pour la page de configuration audio
		 */
		QWidget *m_pageAudio;

		/**
		 * @brief m_pageProps Widget pour la page de propriétés
		 */
		QWidget *m_pageProps;

		/**
		 * @brief m_spacer Un espacement (nécessaire)
		 */
		QSpacerItem* m_spacer;

		/**
		 * @brief m_grid La grille
		 */
		ChordTableWidget* m_grid;
};

#endif // EDITORPANEL_H
