#ifndef ENEMY_H
#define ENEMY_H

/* Includes: 
Base class */
#include "Creature.h"

class Enemy : public Creature {
public:
	/* Declarations: */
	// Constructor
	Enemy();
	// Destructor
	~Enemy();

	// Returns a random number between 0 and the passed limit
	int GetRandom(int numLimit);
	// Returns the enemy's current state (true if cleared)
	bool GetState();
	// Returns the type of encounter (name and description)
	std::vector<std::string> GetEncounter();
	// Returns the available choices in an encounter
	std::vector<std::string> GetEncounterChoices();
	// Returns the enemy's choice in an encounter
	int GetEnemyChoice();
	// Changes the state of an enemy (cleared if the player wins an encounter)
	void SetState();
	// Resets the enemy's encounter and loads a random available one
	void SetEncounter(int difficulty);
	// Reads and stores data related to a specified encounter in the current difficulty
	void LoadEncounter(int difficulty, int encounterIndex);
	// Handles the enemy's movement
	void HandleControls(int randomNum);
	// Draws the enemy as a 2D rectangle in the game window
	void DrawCharacter();

private:
	// Stores the available choices in the currently loaded encounter
	std::vector<std::string> encounterChoices;
	// Stores the name and description of the currently loaded encounter
	std::vector<std::string> enemyEncounter;
	// Stores whether or not this enemy has been cleared
	bool enemyCleared;
	// Stores the file path for the ItemsList file (data stored: Enemy interactions, game tips' names and descriptions)
	std::string itemsFile;

};


#endif