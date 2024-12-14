#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

/* Header files */
#include "Config.h"
#include "Play.h"
#include "Enemy.h"
#include "NPC.h"

/* Includes:
Standard header files */
#include <string>
#include <fstream>
#include <vector>

class LevelManager {

public: 
	/* Declarations: */
	// Constructor
	LevelManager();
	// Destructor
	~LevelManager();

	/* Getter functions: */
	// Returns the entrance and exit
	std::vector<Play::Point2D> GetLevel();
	std::vector<Play::Point2D> GetBoundaries();
	void LoadLevel();

	void CreateObstacles();
	void ManageEnemies(Play::Point2D playerPos, float elapsedTime);

	void SetLevel(int newLevel);
	void SetLevel();
	void SetMode(int newDifficulty);
	
	void PrintLevel();

private: 

	int levelIndex;
	int difficulty;
	bool encounter;
	float secondsTimer;

	Play::Point2D startPos;
	Play::Point2D endPos;
	std::vector<Play::Point2D> enemyPos;
	std::vector<Play::Point2D> trapPos;
	std::vector<Play::Point2D> openTiles;

	std::string TextFile = "Data\\Textfiles\\Levels.txt";
	bool isNew = true;
	std::vector<std::string> levelTiles;

	std::vector<Enemy*> allEnemies;

};

#endif