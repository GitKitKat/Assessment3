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

	Play::Point2D GetLevel();
	void LoadLevel();

	void SetLevel(int newLevel);
	void SetMode(int newDifficulty);
	
	void PrintLevel();

private: 

	std::string levelIndex;
	int difficulty;
	// This variable stores each whole line read from the text file.
	std::string newLine = "";

	Play::Point2D startPos;

	std::string TextFile = "Data\\Textfiles\\Levels.txt";
	bool levelFound = false;
	bool isNew = true;
	std::vector<std::string> levelTiles;

//	int test;

};

#endif