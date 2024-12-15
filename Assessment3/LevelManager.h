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

	std::vector<std::string> GetInteractionLimit(int enemyIndex);
	int GetEnemyChoice(int enemyIndex);
	void ClearEnemy(int enemyIndex);
	void ClearObstacles();
	int GetLevelIndex();
	void CreateObstacles();
	int ManageEnemies(Play::Point2D playerPos, float elapsedTime);
	std::vector<std::string> GetEnemyInteraction(int enemyIndex);
	bool CheckTraps(Play::Point2D playerPos);
	void SetLevel(float newLevel);
	void TrapLevel(int increase);
	int GetRandom(int numLimit);
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
	std::vector<Play::Point2D> keyPos;
	std::vector<Play::Point2D> openTiles;

	std::string TextFile;
	bool isNew = true;
	std::vector<std::string> levelTiles;

	std::vector<Enemy*> allEnemies;

};

#endif