#ifndef SAVEQUEUE_H
#define SAVEQUEUE_H

#include <QStack>
#include "StatePacket.h"

class GridEditor;
class SaveQueue: public QObject
{
		Q_OBJECT
	public:
		explicit SaveQueue(GridEditor *editor);

		StatePacket* currentEditorState();
		bool canUndo();
		bool canRedo();
		void firstSave();
		void clear();
		void emptyQueue(QList<StatePacket*>* q);

	signals:
		void save();

	public slots:
		void saveSlot();

		void saveState(bool redo = false);
		void undo();
		void redo();


	private:
		void restoreState(StatePacket* statePacket);
		GridEditor* m_editor;
		int max_depth;

		QList<StatePacket*> undoQueue;
		QList<StatePacket*> redoQueue;

		bool m_undoState, m_redoState; // sert à éviter des sauvegardes en doublon si on undo plusieurs fois d'affilée
		StatePacket* m_currentState;
		int mod;


};

#endif // SAVEQUEUE_H
