#include "UndoManager.h"
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

/**
 * @brief UndoManager::UndoManager
 * @param editor Pointeur vers l'éditeur.
 *
 * Constructeur
 */
UndoManager::UndoManager(GridEditor* editor) :
	QObject(editor),
	m_editor(editor),
	max_depth(10),
	m_undoState(false),
	m_redoState(false),
	m_currentState(0)
{
	connect(this, SIGNAL(save()), this, SLOT(saveSlot()));
}

UndoManager::~UndoManager()
{
	clear();
}

/**
 * @brief UndoManager::clear
 *
 * Remet les données à 0.
 */
void UndoManager::clear()
{
	emptyList(&undoList);
	emptyList(&redoList);
	delete m_currentState;
	m_currentState = 0;
}

/**
 * @brief UndoManager::saveSlot
 *
 * Appelé pour déclencher une sauvegarde
 */
void UndoManager::saveSlot()
{
	//qDebug() << "saveSlot()";
	saveState(false); // pour les undo
}

/**
 * @brief UndoManager::currentEditorState
 * @return Etat actuel.
 *
 * Permet de renvoyer un StatePacke qui contient l'état actuel de l'éditeur.
 */
StatePacket* UndoManager::currentEditorState()
{
	return new StatePacket(m_editor->m_grid,
						   m_editor->m_audioWindow->getBeginning(), m_editor->m_audioWindow->getBar(),  m_editor->m_audioWindow->getEnd(),
						   m_editor->m_trackProperties->getArtist(), m_editor->m_trackProperties->getTrack(), m_editor->m_trackProperties->getComment(),
						   m_editor->m_trackProperties->getBarSize(),(int) m_editor->m_trackProperties->getTimeSignature());

}

/**
 * @brief UndoManager::firstSave
 * A appeler lorsqu'on charge ou crée un nouveau morceau
 */
void UndoManager::firstSave()
{
	clear();
	m_currentState = currentEditorState();
}

/**
 * @brief UndoManager::saveState
 * @param redo Permet de choisir si on sauve dans la queue redo ou undo.
 *
 * Enregistre un état.
 */
void UndoManager::saveState(bool redo)
{
	if(redo) // on ne doit être dans ce cas que lorsque l'on fait un undo
	{
		if(redoList.size() == max_depth )
		{
			delete redoList.last();
			redoList.removeLast();
		}
		redoList.push_front(m_currentState);
	}
	else
	{
		if(undoList.size() == max_depth )
		{
			delete undoList.last();
			undoList.removeLast();
		}
		undoList.push_front(m_currentState);
		emptyList(&redoList); // on vide la pile de redo pour ne pas avoir plusieurs "branches" différentes

		m_editor->enableRedo(false);
		m_editor->enableUndo(true);
	}


	m_currentState = currentEditorState();

	m_redoState = false;
}


/**
 * @brief UndoManager::emptyList
 * @param q Liste à vider
 */
void UndoManager::emptyList(QList<StatePacket*> *q)
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

/**
 * @brief UndoManager::undo
 *
 * Action effectuée quand on appuie sur undo.
 */
void UndoManager::undo()
{
	if(!m_redoState)
	{
		saveState(true); // on sauve l'état actuel, qui a été modifié dans la queue de Redo
	}
	else
	{
		redoList.push_front(m_currentState);
	}
	m_editor->enableRedo(true);
	m_redoState = true;

	m_currentState = undoList.first();
	undoList.pop_front();
	restoreState(m_currentState);
	if(undoList.empty())
	{
		m_editor->enableUndo(false);
	}
}

/**
 * @brief UndoManager::redo
 *
 * Action effectuée quand on appuie sur redo.
 */
void UndoManager::redo()
{
	// m_currentState est défini car il ne peut y avoir de redo
	// que s'il y a eu un undo et qu'il n'y a pas eu de changement entre temps
	undoList.push_front(m_currentState);
	m_editor->enableUndo(true);

	m_currentState = redoList.first();
	redoList.pop_front();
	restoreState(m_currentState);
	if(redoList.empty())
	{
		m_editor->enableRedo(false);
	}
}

/**
 * @brief UndoManager::restoreState
 * @param statePacket L'état dans lequel remettre l'éditeur
 *
 * Remet l'éditeur dans l'état contenu dans statePacket
 */
void UndoManager::restoreState(StatePacket* statePacket)
{
	m_currentState = statePacket;

	m_editor->createGrid(statePacket->m_grid->columnCount(), statePacket->m_grid->rowCount());
	statePacket->m_grid->deepCopy(m_editor->m_grid);

	connect(m_editor->m_grid, SIGNAL(somethingChanged()), this, SIGNAL(save()));
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


/**
 * @brief UndoManager::canUndo
 * @return Vrai si on peut faire undo
 */
bool UndoManager::canUndo()
{
	return undoList.size() > 0;
}

/**
 * @brief UndoManager::canRedo
 * @return Vrai si on peut faire redo
 */
bool UndoManager::canRedo()
{
	return redoList.size() > 0 && m_redoState;
}
