/*
 * ScoreManager.cpp
 *
 *  Created on: 16 mars 2011
 *      Author: raphael
 */

#include "ScoreManager.h"
#include "GuitarTutor/Chord.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <unistd.h>

#include <QDebug>


ScoreManager::ScoreManager(MusicManager* musicManager) {
	m_musicManager = musicManager;
	m_iscoreEngine = new Engines();

	m_chordControl = new chord_ctrl();

	m_nextPart = "";
	m_currentPart = "";
	m_previousPart = "";

	m_currentPartLaunchTime = 0;

	m_isAScoreToRun = false;

}

ScoreManager::~ScoreManager() {
	//delete m_iscoreEngine;
	delete m_chordControl;

	while (m_musicManager->isRunning()) {
		usleep(100);
	}

	delete m_musicManager;
}

QStringList ScoreManager::getCurrentChord()
{
    return (m_currentInputChord.at(0) == "nc")? QStringList(" ") : m_currentInputChord;
}

bool ScoreManager::isCurrentChordValidated()
{
	return true;//m_validatedNoteInCurrentPart[m_currentNoteId];
}

void triggerCallBack(void* arg, bool state, unsigned int /*triggerId*/, unsigned int /*boxId*/, unsigned int /*controlPointIndex*/, std::string waitedString)
{
	ScoreManager* scoreManager = (ScoreManager*) arg;

	if (state) {
		if (waitedString != SCORE_END) {
			scoreManager->m_iscoreEngine->simulateNetworkMessageReception(scoreManager->m_nextPart);
			scoreManager->m_validatedNoteInCurrentPart.clear();
		} else {
			//scoreManager->stop();
		}
	} else {
		if (waitedString == scoreManager->m_nextPart) {
			scoreManager->m_previousPart = scoreManager->m_currentPart;
			scoreManager->m_currentPart = scoreManager->m_nextPart;

			scoreManager->m_currentPartLaunchTime = scoreManager->m_iscoreEngine->getCurrentExecutionTime();
		}
		std::cout << waitedString << std::endl;
	}
}

void crossAction(void* arg, unsigned int boxNb, unsigned int controlPointIndex, std::vector<unsigned int>)
{
	ScoreManager* scoreManager = (ScoreManager*) arg;

	bool isAPart = ((scoreManager->m_partsBeginInMs.find(boxNb)) != scoreManager->m_partsBeginInMs.end());
	bool isANote = ((scoreManager->m_scoreNotes.find(boxNb)) != scoreManager->m_scoreNotes.end());

	if (isAPart && (controlPointIndex == BEGIN_CONTROL_POINT_INDEX)) {
		scoreManager->goToInMs(scoreManager->m_partsBeginInMs[boxNb] * 1000);
	} else if (isANote && (controlPointIndex == BEGIN_CONTROL_POINT_INDEX)) {
		scoreManager->m_currentNote = scoreManager->m_scoreNotes[boxNb];
		scoreManager->m_currentNoteId = boxNb;
		scoreManager->m_currentNoteDurationInScore = scoreManager->m_iscoreEngine->getBoxDuration(boxNb);
		scoreManager->m_lastMatchingChordsCheckTime = scoreManager->m_iscoreEngine->getCurrentExecutionTime();
		scoreManager->m_matchingChordsTimeInNote = 0;

		std::cout << "START " << boxNb - 2 << " " << scoreManager->m_currentNote << std::endl;
	} else if (isANote && (controlPointIndex == END_CONTROL_POINT_INDEX)) {

	} else if (isAPart && (controlPointIndex == END_CONTROL_POINT_INDEX)) {
		scoreManager->m_iscoreEngine->produceNewToken();
	} else if ((boxNb == scoreManager->m_mainBoxId) && (controlPointIndex == END_CONTROL_POINT_INDEX)) {
		scoreManager->m_musicManager->stop();
		//std::cout << "c'est la fin" << std::endl;
	} else if ((boxNb == scoreManager->m_mainBoxId) && (controlPointIndex == BEGIN_CONTROL_POINT_INDEX)) {

	}
}

void ScoreManager::update()
{
	double buffer[INPUT_FRAMES_PER_BUFFER];
	chord_init(m_chordControl, SAMPLE_RATE, INPUT_FRAMES_PER_BUFFER, INPUT_FRAMES_PER_BUFFER);

	m_musicManager->fillBufferWithLastInputValues(buffer, INPUT_FRAMES_PER_BUFFER);

	/* Process */
	chroma_compute(m_chordControl, buffer, INPUT_FRAMES_PER_BUFFER);

    m_currentInputChord = BasicChord::convertChordToStringList(chord_compute(m_chordControl));

	// ci-dessous, la partie pour la validation qui permet de continuer ou non dans la partie suivante.
	// à remettre quand ça fonctionnera.

	//qDebug() << QString(m_currentInputChord.c_str()) << "-" << QString(m_currentNote.c_str());
	/*if (m_isAScoreToRun) {
		unsigned int currentTime = m_iscoreEngine->getCurrentExecutionTime();

		if (m_currentInputChord == m_currentNote) {
			m_matchingChordsTimeInNote += (currentTime - m_lastMatchingChordsCheckTime);
			qDebug() << (m_matchingChordsTimeInNote * 100.0)/m_currentNoteDurationInScore;
			if (((m_matchingChordsTimeInNote * 100.0)/m_currentNoteDurationInScore) > PERCENT_TO_VALIDATE_NOTE) {
				m_validatedNoteInCurrentPart[m_currentNoteId] = true;
			}
		}

		m_lastMatchingChordsCheckTime = currentTime;
	}*/
}

unsigned int ScoreManager::getValidatedNotesPercent()
{
	if (m_isAScoreToRun) {
		unsigned int currentPartBoxId = m_partNameToBoxId[m_currentPart];
		std::vector<unsigned int> boxesId;

		m_iscoreEngine->getBoxesId(currentPartBoxId, boxesId);

		//	std::cout << "nb validated note " << m_validatedNoteInCurrentPart.size() << std::endl;

		return((m_validatedNoteInCurrentPart.size() * 100.0)/boxesId.size());
	} else {
		return 0;
	}
}

void ScoreManager::setToNaturalNextPart()
{
	m_nextPart = m_nextPartName[m_currentPart];
}

void ScoreManager::getScore(prioritizedScore& score, int relativeBegin, int relativeEnd)
{
	if (m_currentPart == SCORE_END) {
		return;
	}

	if (!m_isAScoreToRun) {
		return;
	}

	std::vector<unsigned int> boxesId;
	unsigned int currentPartBoxId = m_partNameToBoxId[m_currentPart];

	unsigned int currentPartBoxDuration = m_iscoreEngine->getBoxDuration(currentPartBoxId);
	m_iscoreEngine->getBoxesId(currentPartBoxId, boxesId);

	int offset = m_iscoreEngine->getCurrentExecutionTime() - m_currentPartLaunchTime;

	for (unsigned int i = 0; i < boxesId.size(); ++i) {
		unsigned int currentId = boxesId[i];

		int currentBoxRelativeBegin = m_iscoreEngine->getBoxBeginTime(currentId) - offset;
		int currentBoxRelativeEnd = m_iscoreEngine->getBoxEndTime(currentId) - offset;

		if (!(currentBoxRelativeBegin > relativeEnd) && !(currentBoxRelativeEnd < relativeBegin)) {
			PrioritizedNote currentNote;

			currentNote.isInCurrentPart = true;
			currentNote.chord = m_scoreNotes[currentId];
			currentNote.relativeBegin = currentBoxRelativeBegin;
			currentNote.relativeEnd = currentBoxRelativeEnd;
			currentNote.isValidated = false;

			//if (currentId == m_currentNoteId) {
			std::map<unsigned int, bool>::iterator it = m_validatedNoteInCurrentPart.find(currentId);
			if (it != m_validatedNoteInCurrentPart.end()) {
				currentNote.isValidated = true;
			}
			//}

			score.push(currentNote);
		}
	}

	if (m_nextPart != SCORE_END) {
		boxesId.clear();

		unsigned int nextPartBoxId = m_partNameToBoxId[m_nextPart];
		m_iscoreEngine->getBoxesId(nextPartBoxId, boxesId);

		for (unsigned int i = 0; i < boxesId.size(); ++i) {
			unsigned int currentId = boxesId[i];

			int currentBoxRelativeBegin = m_iscoreEngine->getBoxBeginTime(currentId)  + currentPartBoxDuration - offset;
			int currentBoxRelativeEnd = m_iscoreEngine->getBoxEndTime(currentId)  + currentPartBoxDuration - offset;

			if (!(currentBoxRelativeBegin > relativeEnd) && !(currentBoxRelativeEnd < relativeBegin)) {
				PrioritizedNote currentNote;

				currentNote.isInCurrentPart = false;
				currentNote.chord = m_scoreNotes[currentId];
				currentNote.relativeBegin = currentBoxRelativeBegin;
				currentNote.relativeEnd = currentBoxRelativeEnd;
				currentNote.isValidated = false;

				score.push(currentNote);
			}
		}
	}

	if (m_previousPart != "") {
		boxesId.clear();

		unsigned int previousPartBoxId = m_partNameToBoxId[m_previousPart];
		unsigned int previousPartBoxDuration = m_iscoreEngine->getBoxDuration(previousPartBoxId);

		m_iscoreEngine->getBoxesId(previousPartBoxId, boxesId);

		for (unsigned int i = 0; i < boxesId.size(); ++i) {
			unsigned int currentId = boxesId[i];

			int currentBoxRelativeBegin = m_iscoreEngine->getBoxBeginTime(currentId) - previousPartBoxDuration - offset;
			int currentBoxRelativeEnd = m_iscoreEngine->getBoxEndTime(currentId) - previousPartBoxDuration - offset;

			if (!(currentBoxRelativeBegin > relativeEnd) && !(currentBoxRelativeEnd < relativeBegin)) {
				PrioritizedNote currentNote;

				currentNote.isInCurrentPart = false;
				currentNote.chord = m_scoreNotes[currentId];
				currentNote.relativeBegin = currentBoxRelativeBegin;
				currentNote.relativeEnd = currentBoxRelativeEnd;
				currentNote.isValidated = false;

				score.push(currentNote);
			}
		}
	}
}

void ScoreManager::goToInMs(unsigned int time)
{
	m_musicManager->goToInMs(time);
}

bool ScoreManager::loadScore(std::string fileName)
{
	std::fstream file;
	file.open(fileName.data());



	NoteData currentNote;

	unsigned int currentBoxId = NO_ID;

	m_mainBoxId = m_iscoreEngine->addBox(0, 500000, ROOT_BOX_ID);
	m_iscoreEngine->addCrossingCtrlPointCallback(this, crossAction);
	m_iscoreEngine->addCrossingTrgPointCallback(this, triggerCallBack);

	unsigned int mainTriggerPointIdEnd = m_iscoreEngine->addTriggerPoint(ROOT_BOX_ID);
	m_iscoreEngine->setTriggerPointMessage(mainTriggerPointIdEnd, SCORE_END);
	m_iscoreEngine->assignCtrlPointToTriggerPoint(mainTriggerPointIdEnd, m_mainBoxId, END_CONTROL_POINT_INDEX);

	float currentRelativeStart = 0;
	float currentAbsoluteStart = 0;

	std::string previousPart = " ";

	if (file) {
		while (!file.eof()) {
			std::string currentLine;
			std::getline(file, currentLine);

			if (currentLine.size() > 0) {

				if (currentLine[0] == '[') {
					if (currentAbsoluteStart != 0) {
						std::vector<unsigned int> movedBox;
						m_iscoreEngine->performBoxEditing(currentBoxId, 0, currentRelativeStart * 1000, movedBox);
					}

					currentRelativeStart = 0;
					currentBoxId = m_iscoreEngine->addBox(0, 400000, m_mainBoxId);

					m_partsBeginInMs[currentBoxId] = currentAbsoluteStart;

					unsigned int currentTriggerPointId = m_iscoreEngine->addTriggerPoint(m_mainBoxId);
					m_iscoreEngine->setTriggerPointMessage(currentTriggerPointId, currentLine);
					m_iscoreEngine->assignCtrlPointToTriggerPoint(currentTriggerPointId, currentBoxId, BEGIN_CONTROL_POINT_INDEX);

					m_partNameToBoxId[currentLine] = currentBoxId;

					if (previousPart != " ") {
						m_nextPartName[previousPart] = currentLine;
						std::cout << previousPart << " " << currentLine << std::endl;
					}

					previousPart = currentLine;
				} else {
					std::istringstream currentStream(currentLine.data());
					currentStream >> currentNote.duration >> currentNote.name;

					m_scoreNotes[m_iscoreEngine->addBox(currentRelativeStart * 1000, currentNote.duration * 1000, currentBoxId)] = currentNote.name;

					currentRelativeStart += currentNote.duration;
					currentAbsoluteStart += currentNote.duration;
				}
			}
		}
	}

	m_isAScoreToRun = true;

	return true;
}

std::string ScoreManager::ScoreToString(LogicalTrack* trackName )
{
	NoteData currentNote, prevNote;
	float currentRelativeStart;
	std::stringstream text("");
	QList<PartTrack*>::iterator it1;
	QList<TrackChord*>::iterator it2;

	QList<PartTrack*> partTrackList = trackName->getPartTrackList();

	for(it1 = partTrackList.begin(); it1 != partTrackList.end(); ++it1)
	{
		std::string current((*it1)->getPartName().toStdString());
		text << "[" << (*it1)->getPartName().toStdString() << "]" << "\n";

		QList<TrackChord*> gtc = (*it1)->getTrackChordsList(); //utilisée dans la boucle qui suit, plante si pas de passage par variable intermédiaire (pourquoi?) --- hamid

		for(it2 = gtc.begin(); it2 != gtc.end(); ++it2)
		{
			//for(int i = 0; i < (*it2)->getRepetition(); i++) //les répétitions sont désactivées car ça foutait la merde.
			{
				QString str_tmp("");
				std::string s("");
				std::string str("");

				s += (*it2)->getChord().toStdString();
				str_tmp.setNum((*it2)->getDuration());
				str += str_tmp.toStdString() + " " + s;

				std::istringstream currentStream(str.data());
				prevNote = currentNote;
				currentStream >> currentNote.duration >> currentNote.name;

				currentRelativeStart = (currentNote.duration - prevNote.duration) / 1000;

				if(currentNote.name != "n")
					text << currentRelativeStart << " " << currentNote.name  << std::endl;
			}
		}
	}
	return text.str();
}

bool ScoreManager::loadScore(LogicalTrack* trackName)
{
	logicalTrack = trackName;

	std::ofstream file("tmp", std::ios::out);
	file << ScoreToString(trackName);
	file.close();
	return loadScore("tmp");
}

bool ScoreManager::isRunning()
{
	return m_musicManager->isRunning();
}

void ScoreManager::run()
{
	qDebug() << "ScoreManager::run()";
	if (m_isAScoreToRun)
	{
		//TODO: [] obligatoires pour les noms de parties si on utilise le fichier tmp
		m_nextPart = "[" + logicalTrack->getPartName(1).toStdString() + "]";
		m_iscoreEngine->play();
		m_musicManager->run();

	}
	else
	{
		qDebug() << "ScoreManager::run() -> else";
		m_iscoreEngine->play();
		goToInMs(0);
	}
	m_musicManager->play();
}
void ScoreManager::pause()
{
	qDebug() << "ScoreManager::stop()";
	//m_musicManager->setMustStop(true);
	m_musicManager->pause();
	m_iscoreEngine->stop();
	m_isAScoreToRun = false;
}

void ScoreManager::stop()
{
	qDebug() << "ScoreManager::stop()";
	m_musicManager->setMustStop(true);
	m_iscoreEngine->stop();

}

std::string ScoreManager::getNextPart() const
{
	return m_nextPart;
}

void ScoreManager::setNextPart(std::string m_nextPart)
{
	this->m_nextPart = m_nextPart;
}

std::string ScoreManager::getCurrentPart() const
{
	return m_currentPart;
}

