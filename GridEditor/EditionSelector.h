#ifndef EDITIONSELECTOR_H
#define EDITIONSELECTOR_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include "NewGridDialog.h"

#define NEW_GRID 0
#define OPEN_GRID 1

/**
 * @brief Fenêtre d'assistant de sélection d'éditeur (à l'ouverture)
 */
class EditionSelector : public QWidget
{
		Q_OBJECT

	public:
		explicit EditionSelector(QWidget *parent);
		~EditionSelector();

	private:
		/**
		 * @brief m_presentation Titre
		 */
		QLabel *m_presentation;

		/**
		 * @brief m_tabs Tabs qui vont contenir les affichages différents
		 */
		QTabWidget *m_tabs;

		/**
		 * @brief m_mainLayout layout général
		 */
		QVBoxLayout *m_mainLayout;

		/**
		 * @brief m_vboxNew layout pour la page "Nouvelle grille"
		 */
		QVBoxLayout *m_vboxNew;

		/**
		 * @brief m_vboxOpen layout pour la page "Ouvrir une grille"
		 */
		QVBoxLayout *m_vboxOpen;

		/**
		 * @brief m_pageNew Widget de la page "Nouvelle grille"
		 */
		QWidget *m_pageNew;

		/**
		 * @brief m_pageOpen Widget de la page "Ouvrir une grille"
		 */
		QWidget *m_pageOpen;
};

#endif // EDITIONSELECTOR_H
