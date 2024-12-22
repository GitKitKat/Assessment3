#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

/* Includes:
Standard header files */
#include <string>
#include <fstream>
#include <vector>

/* Header files */
#include "Config.h"
#include "Play.h"
#include "Enemy.h"

class LevelManager {
public:
	/* Declarations: */
	// Constructor
	LevelManager();
	// Destructor
	~LevelManager();

	// Returns a random number between 0 and the passed limit
	int GetRandom(int numLimit);
	// Changes the difficulty mode to the passed amount
	void SetMode(int newDifficulty);

	/* Regarding levels */
	// Returns the ID of the currently loaded level
	int GetLevelIndex();
	// Returns the level's entrance and exit
	std::vector<Play::Point2D> GetLevel();
	// Returns the passable tiles's positions
	std::map<float, std::vector<Play::Point2D>> GetBoundaries();
	// Changes the current level ID to a passed amount
	void SetLevel(float newLevel);
	// Increases the level ID by 1
	void SetLevel();
	// Returns whether the player has stepped on a 'trap' or 'key' tile
	bool CheckTraps(Play::Point2D playerPos);
	// Changes the current level ID by 10 (if the new level was reached by a trap, increased by 10; if by a key, decreased by 10)
	void TrapLevel(int increase);
	// Reads and stores a specified level from the Levels textfile
	void LoadLevel();
	// Draws the loaded level in the game window
	void PrintLevel();

	/* Regarding enemies */
	// Creates and stores instances of the enemy class (for as many enemies are marked in a level)
	void CreateEnemies();
	// Handles the movement and player collision of all enemies on a level
	int ManageEnemies(Play::Point2D playerPos, float elapsedTime);
	// Returns the name and description of an ongoing enemy encounter
	std::vector<std::string> GetEnemyEncounter(int enemyIndex);
	// Returns the available choices in an ongoing enemy encounter
	std::vector<std::string> GetEncounterChoices(int enemyIndex);
	// Returns the enemy's choice in an encounter
	int GetEnemyChoice(int enemyIndex);
	// Returns whether or not the 'Leaderless' trophy is claimed (cleared all enemies in at least 7 levels)
	bool GetLeaderlessClear();
	// Returns whether or not the 'Pacifist' trophy is claimed (cleared the game without defeating any enemies)
	bool GetPacifistClear();
	// Changes the encountered enemy's state to 'cleared'
	void ClearEnemy(int enemyIndex);
	// Deletes all enemy instances
	void DeleteEnemies();

private:
	// Stores the ID of the currently loaded level
	int levelIndex;
	// Stores the current difficulty mode (i.e. 1 for EASY)
	int difficulty;
	// Whether or not the player encountered an enemy
	bool encounter;
	// Stores the amount of time passed since an enemy attempted to move
	float secondsTimer;
	// Stores the position of a level's entrance
	Play::Point2D startPos;
	// Stores the location of a level's exit
	Play::Point2D endPos;
	// Stores the starting positions of all enemies on a level
	std::vector<Play::Point2D> enemyPos;
	// Stores the positions of all 'trap' tiles (changes the level layout)
	std::vector<Play::Point2D> trapPos;
	// Stores the positions of all 'key' tiles (returns the player to the level before a trap activated)
	std::vector<Play::Point2D> keyPos;
	// Stores all of the passable tiles in a level
	std::map<float, std::vector<Play::Point2D>> openTiles;
	// Stores all of the tiles in a level (including walls, passable tiles, traps, changing tiles and enemy starting positions)
	std::vector<std::string> levelTiles;
	// Stores all the instances of an enemy in a level and handles them
	std::vector<Enemy*> allEnemies;
	// Stores the number of enemies that were cleared in a level
	int clearedEnemies;
	// Stores the number of times all the enemies on a level were cleared
	int clearedLevel;
	// Whether or not any enemies were cleared during the game
	bool defeatedEnemies;
	// Stores the file path for the Levels file (data stored: level ID, mode, traps, entrance, exit and randomly changing tiles)
	std::string textFile;

};

#endif