#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <QStack>
#include "StatePacket.h"

class GridEditor;
/**
 * @brief Gestionnaire d'undo / redo
 */
class UndoManager: public QObject
{
		Q_OBJECT
	public:
		explicit UndoManager(GridEditor *editor);
		~UndoManager();

		StatePacket* currentEditorState();
		bool canUndo();
		bool canRedo();
		void firstSave();
		void clear();
		void emptyList(QList<StatePacket*>* );

	signals:
		/**
		 * @brief save est émis lorsqu'on veut sauver une action qui vient d'être effectuée
		 */
		void save();

	public slots:
		void saveSlot();

		void saveState(bool redo = false);
		void undo();
		void redo();


	private:
		void restoreState(StatePacket*);

		/**
		 * @brief m_editor Pointeur vers l'éditeur pour faciliter les sauvegardes (pour faire proprement, faire 25 getters / setters :) )
		 */
		GridEditor* m_editor;

		/**
		 * @brief max_depth Profondeur d'undo / redo maximum
		 */
		int max_depth;

		/**
		 * @brief undoList Liste dans laquelle sont stockés les paquets de l'undo
		 */
		QList<StatePacket*> undoList;

		/**
		 * @brief redoList Liste dans laquelle sont stockés les paquets du redo
		 */
		QList<StatePacket*> redoList;

		/**
		 * @brief m_undoState sert à éviter des sauvegardes en doublon si on redo plusieurs fois d'affilée
		 */
		bool m_undoState;

		/**
		 * @brief m_redoState sert à éviter des sauvegardes en doublon si on undo plusieurs fois d'affilée
		 */
		bool m_redoState;

		/**
		 * @brief m_currentState Représente l'état actuel de l'éditeur, tel qu'il est sous les yeux de la personne qui édite.
		 */
		StatePacket* m_currentState;
};

#endif // UNDOMANAGER_H
