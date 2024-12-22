#ifndef NPC_H
#define NPC_H

/* Includes:
Base class */
#include "Creature.h"

class NPC : public Creature {
public:
	/* Declarations: */
	// Constructor
	NPC();
	// Destructor
	~NPC();

	// Checks whether the NPC is in the same tile as the player
	bool CheckCollision(Play::Point2D playerPos);
	// Returns the current dialogue (each line is a different string)
	std::vector<std::string> GetDialogueDesc();
	// Returns the available options (player responses) for the current dialogue
	std::vector<std::string> GetDialogueOptions();
	// Changes the current dialogue by the amount passed
	void SetDialogue(int increase);
	// Changes the current dialogue to the passed dialogue (ID)
	void ResetDialogue(int currentLevel, int increase);
	// Returns whether the current conversation has ended (breakpoint in the dialogue)
	bool NextDialogue();
	// Returns whether the current dialogue has ended (ends the NPC level)
	bool DialogueEnd();
	// Draws the NPC as a 2D rectangle in the game window
	void DrawCharacter();
	// Reads and stores data related to the current dialogue
	void LoadDialogue(int currentLevel);
	// Draws the current dialogue and any available choices on the game screen
	int PrintDialogue(bool currentControls);

private:
	// Stores the ID of the currently loaded dialolgue
	int dialogueIndex;
	// Stores the file path for the Dialogues file (data stored: speaking NPC's name, dialogue descriptions, player choices)
	std::string itemsFile;
	// Stores each line of the current dialogue
	std::vector<std::string> dialogueDesc;
	// Stores available options (player responses) to the current dialogue 
	std::vector<std::string> dialogueChoices;
	// Whether the current dialogue marks the end of a conversation
	bool earlyExit;
	// Whether the current level has been completed (dialogue read until the end)
	bool levelComplete;

};

#endif