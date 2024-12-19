#ifndef NPC_H
#define NPC_H

/* Includes: 
Standard header files */
#include <string>

#include "Creature.h"

class NPC : public Creature {

public:
	/* Declarations: */
	// Constructor
	NPC();
	// Destructor
	~NPC();

	std::vector<std::string> GetDialogueDesc();
	std::vector<std::string> GetDialogueOptions();

	void SetDialogue(int increase);
	void ResetDialogue(int currentLevel, int increase);
	bool DialogueEnd();
	bool NextDialogue();
	void LoadDialogue(int currentLevel);
	int PrintDialogue(bool currentControls);

	// Check whether the NPC is in the same tile as the player
	bool CheckCollision(Play::Point2D playerPos);
	// Draws the NPC as a 2D rectangle in the game window
	void DrawCharacter();

private:

	bool earlyExit;
	int dialogueIndex;
	std::string itemsFile;
	std::vector<std::string> dialogueDesc;
	std::vector<std::string> dialogueChoices;
	bool levelComplete;
	std::vector<Play::Point2D> NPCPos;

};

#endif