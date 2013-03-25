#include "SaveQueue.h"
#include "GridEditor.h"

/*
 * Quand quelque chose change, on le met dans l'état actuel m_currentState.
 * L'état actuel précédent part dans l'undoQueue.
 * Si on fait undo : l'état actuel m_currentState part dans la redoQueue et m_currentState prend la tête d'undo.
 * Si on fait redo : l'état actuel m_currentState part dans l'undoQueue et m_currentState prend la tête de redo.
 *
 *
 *
 */
SaveQueue::SaveQueue(GridEditor* editor) :
	QObject(editor),
	m_editor(editor),
	max_depth(10),
	m_undoState(false),
	m_redoState(false),
	m_currentState(0),
	mod(0)
{
	connect(this, SIGNAL(save()), this, SLOT(saveSlot()));
}

void SaveQueue::clear()
{
	emptyQueue(&undoQueue);
	emptyQueue(&redoQueue);
}

void SaveQueue::saveSlot()
{
	mod = (mod + 1) % 2;
	if(mod)
	{
		saveState(false); // pour les undo
	}
}

StatePacket* SaveQueue::currentEditorState()
{
	return new StatePacket(m_editor->grid,
						   m_editor->audioWindow->getBeginning(), m_editor->audioWindow->getBar(),  m_editor->audioWindow->getEnd(),
						   m_editor->trackProperties->getArtist(), m_editor->trackProperties->getTrack(), m_editor->trackProperties->getComment(),
						   m_editor->trackProperties->getBarSize(),(int) m_editor->trackProperties->getTimeSignature());

}

void SaveQueue::firstSave()
{
	emptyQueue(&undoQueue);
	emptyQueue(&redoQueue);
	m_currentState = currentEditorState();
}

// enregistrement d'un nouvel état.
void SaveQueue::saveState(bool redo)
{
	if(redo) // on ne doit être dans ce cas que lorsque l'on fait un undo
	{
		if(redoQueue.size() == max_depth )
		{
			delete redoQueue.last();
			redoQueue.removeLast();
		}
		redoQueue.push_front(m_currentState);
	}
	else
	{
		if(undoQueue.size() == max_depth )
		{
			delete undoQueue.last();
			undoQueue.removeLast();
		}
		undoQueue.push_front(m_currentState);
		emptyQueue(&redoQueue); // on vide la pile de redo pour ne pas avoir plusieurs branches différentes

		m_editor->enableRedo(false);
		m_editor->enableUndo(true);
	}


	m_currentState = currentEditorState();

	m_redoState = false;
}

void SaveQueue::emptyQueue(QList<StatePacket*> *q)
{
	while(!q->empty())
	{
		if(q->last() != 0)
		{
			delete q->last();
		}
		q->removeLast();
	}
}

void SaveQueue::undo()
{
	if(!m_redoState)
	{
		saveState(true); // on sauve l'état actuel, qui a été modifié dans la queue de Redo
	}
	else
	{
		redoQueue.push_front(m_currentState);
	}
	m_editor->enableRedo(true);
	m_redoState = true;

	m_currentState = undoQueue.first();
	undoQueue.pop_front();
	restoreState(m_currentState);
	if(undoQueue.empty())
	{
		m_editor->enableUndo(false);
	}
}

void SaveQueue::redo()
{
	// m_currentState est défini car il ne peut y avoir de redo
	// que s'il y a eu un undo et qu'il n'y a pas eu de changement entre temps
	undoQueue.push_front(m_currentState);
	m_editor->enableUndo(true);

	m_currentState = redoQueue.first();
	redoQueue.pop_front();
	restoreState(m_currentState);
	if(redoQueue.empty())
	{
		m_editor->enableRedo(false);
	}
}

void SaveQueue::restoreState(StatePacket* statePacket)
{
	m_currentState = statePacket;

	m_editor->createGrid(statePacket->grid->columnCount(), statePacket->grid->rowCount(), true);
	statePacket->grid->deepCopy(m_editor->grid);

	connect(m_editor->grid, SIGNAL(somethingChanged()), this, SIGNAL(save()));
	/*  Tout ça est à réactiver quand la sauvegarde sera dispo pour le reste
	 * -> il faut vérifier qu'on ne sauve pas à chaque changement de valeur sinon ça va faire n'importe quoi
	 * -> utiliser mouseReleasesEvent notamment
	m_editor->audioWindow->setBeginning(statePacket->beginning);
	m_editor->audioWindow->setBar(statePacket->bar);
	m_editor->audioWindow->setEnd(statePacket->end);

	m_editor->trackProperties->setArtist(statePacket->artist);
	m_editor->trackProperties->setTrack(statePacket->song);
	m_editor->trackProperties->setComment(statePacket->comment);
	m_editor->trackProperties->setBarSize(statePacket->chordsPerBar);
	m_editor->trackProperties->setTimeSignature(statePacket->timeSignature);
	*/
}



bool SaveQueue::canUndo()
{
	return undoQueue.size() > 0;
}

bool SaveQueue::canRedo()
{
	return redoQueue.size() > 0 && m_redoState;
}
