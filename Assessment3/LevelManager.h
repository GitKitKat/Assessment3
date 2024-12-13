#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Config.h"
#include "Play.h"
#include "Enemy.h"

#include <string>
#include <fstream>
#include <vector>

class LevelManager {

public: 
	LevelManager();
	~LevelManager();

	std::vector<Play::Point2D> GetLevel();
	std::vector<Play::Point2D> GetBoundaries();
	void LoadLevel();

	void CreateObstacles();

	void SetLevel(int newLevel);
	void SetLevel();
	void SetMode(int newDifficulty);
	
	void PrintLevel();

private: 

	int levelIndex;
	int difficulty;

	Play::Point2D startPos;
	Play::Point2D endPos;
	std::vector<Play::Point2D> enemyPos;
	std::vector<Play::Point2D> trapPos;
	std::vector<Play::Point2D> openTiles;

	std::string TextFile = "Data\\Textfiles\\Levels.txt";
	bool isNew = true;
	std::vector<std::string> levelTiles;

	Enemy* levelEnemy = new Enemy();

};

#endif