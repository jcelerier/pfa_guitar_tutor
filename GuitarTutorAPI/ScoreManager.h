/*
 * ScoreManager.h
 *
 *  Created on: 16 mars 2011
 *      Author: raphael
 */

#ifndef SCOREMANAGER_H_
#define SCOREMANAGER_H_

#include "IScoreLight/Includes/Engines.hpp"
#include "MusicManager.h"
#include "Track/LogicalTrack.h"

#include <string>
#include <map>
#include <vector>

#include <queue>

#define SCORE_END "[#END#]"
#define PERCENT_TO_VALIDATE_NOTE 30

class NoteData
{
public:
	float duration;
	std::string name;
};

class PrioritizedNote
{
public:
	int relativeBegin;
	int relativeEnd;
	std::string chord;
	bool isInCurrentPart;
	bool isValidated;


	bool operator() (const PrioritizedNote& left, const PrioritizedNote& right) {
		  return (left.relativeBegin > right.relativeBegin);
	  }
};

typedef std::priority_queue<PrioritizedNote, std::vector<PrioritizedNote, std::allocator<PrioritizedNote> >, PrioritizedNote> prioritizedScore;

class ScoreManager {
public:
	ScoreManager(MusicManager*);
	virtual ~ScoreManager();

	std::string getCurrentChord();

	bool loadScore(std::string fileName);
    bool loadScore(LogicalTrack* trackName);

	void run();

	bool isRunning();
	void goToInMs(unsigned int time);

	friend void crossAction(void* arg, unsigned int boxNb, unsigned int controlPointIndex, std::vector<unsigned int>);
	friend void triggerCallBack(void* arg, bool state, unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex, std::string waitedString);
    std::string getNextPart() const;
    void setNextPart(std::string m_nextPart);
    std::string getCurrentPart() const;

    unsigned int getValidatedNotesPercent();
    void setToNaturalNextPart();

    void getScore(prioritizedScore& score, int relativeBegin, int relativeEnd);

    void update();

    void saveRecordedData(std::string fileName) {
    	m_musicManager->saveRecordedData(fileName);
    }

    void stop();

private:
	MusicManager* m_musicManager;
	Engines* m_iscoreEngine;
	std::string m_currentNote;
	unsigned int m_currentNoteId;
	std::string m_currentInputChord;


	bool m_isAScoreToRun;

	std::map<std::string, std::string> m_nextPartName;


	unsigned int m_currentNoteDurationInScore;
	unsigned int m_lastMatchingChordsCheckTime;
	unsigned int m_matchingChordsTimeInNote;

	unsigned int m_currentPartLaunchTime;

	std::map<unsigned int, bool> m_validatedNoteInCurrentPart;

	chord_ctrl *m_chordControl;

	std::string m_nextPart;
	std::string m_currentPart;
	std::string m_previousPart;

	std::map<unsigned int, std::string> m_scoreNotes;

	std::map<unsigned int, float> m_partsBeginInMs;
	std::map<std::string, unsigned int> m_partNameToBoxId;

	unsigned int m_mainBoxId;

	std::vector<float> m_parts;
};

#endif /* SCOREMANAGER_H_ */
