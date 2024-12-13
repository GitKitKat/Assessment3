#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Config.h"
#include "Play.h"
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

	void SetLevel(int newLevel);
	void SetMode(int newDifficulty);
	
	void PrintLevel();

private: 

	std::string levelIndex;
	int difficulty;

	Play::Point2D startPos;
	Play::Point2D endPos;
	std::vector<Play::Point2D> openTiles;

	std::string TextFile = "Data\\Textfiles\\Levels.txt";
	bool levelFound = false;
	bool isNew = true;
	std::vector<std::string> levelTiles;

};

#endif