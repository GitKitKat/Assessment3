#ifndef NPC_H
#define NPC_H

/* Includes: 
Standard header files */
#include <string>

#include "Creature.h"

class NPC : public Creature {

public:
	NPC();
	~NPC();

	std::vector<std::string> GetDialogueDesc();
	std::vector<std::string> GetDialogueOptions();

	void SetDialogue();
	void SetDialogue(int currentLevel);
	void LoadDialogue(int currentLevel);
	void PrintDialogue();

	bool CheckCollision(Play::Point2D playerPos);
	void DrawCharacter() override;

private:

	int dialogueIndex;
	std::string itemsFile;
	std::vector<std::string> dialogueDesc;
	std::vector<std::string> dialogueChoices;
	bool levelComplete;
	std::vector<Play::Point2D> NPCPos;

};

#endif