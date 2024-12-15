/* Includes: 
* This file's header */
#include "LevelManager.h"

/* Header files */
#include "Play.h"

LevelManager::LevelManager() {
	/* Definitions */
	// 
	levelIndex = 0;
	difficulty = 1;
	startPos = { 0.0f, 0.0f };
	endPos = { 0.0f, 0.0f };
	encounter = false;
	secondsTimer = 0.0f;
	TextFile = "Data\\Textfiles\\Levels.txt";

}

LevelManager::~LevelManager() {
	// Destructor
}

std::vector<Play::Point2D> LevelManager::GetBoundaries() {

	std::vector<Play::Point2D> tempVector = openTiles;
	return tempVector;

}

int LevelManager::GetLevelIndex() {
	return levelIndex;
}

void LevelManager::ClearObstacles() {

	for (int i = 0; i < allEnemies.size(); i++) {
		delete allEnemies[i];
	}
	allEnemies.clear();

}

void LevelManager::CreateObstacles() {

	if (allEnemies.size() > 0 && allEnemies.size() != enemyPos.size()) {
		ClearObstacles();
	}

	if (enemyPos.size() > 0 && allEnemies.size() == 0) {

		for (int i = 0; i < enemyPos.size(); i++) {

			Enemy* roamingEnemy = new Enemy();
			roamingEnemy->SetBoundaries(GetBoundaries());
			roamingEnemy->SetPosition(enemyPos[i]);
			roamingEnemy->SetInteraction(difficulty);
			allEnemies.push_back(roamingEnemy);

		}

	}
	else if (enemyPos.size() > 0 && allEnemies.size() > 0) {
		for (int i = 0; i < enemyPos.size(); i++) {
			allEnemies[i]->SetInteraction(difficulty);
		}
	}

}

bool LevelManager::CheckTraps(Play::Point2D playerPos) {

	if (trapPos.size() > 0) {

		for (int i = 0; i < trapPos.size(); i++) {

			if (trapPos[i] == playerPos) {

				TrapLevel(10);
				return true;

			}

		}
	} 
	if (keyPos.size() > 0) {

		for (int i = 0; i < keyPos.size(); i++) {

			if (keyPos[i] == playerPos) {

				TrapLevel(-10);
				return true;

			}

		}
	}

	return false;

}

std::vector<std::string> LevelManager::GetEnemyInteraction(int enemyIndex) {

	return allEnemies[enemyIndex]->GetInteraction();

}

std::vector<std::string> LevelManager::GetInteractionLimit(int enemyIndex) {

	return allEnemies[enemyIndex]->InteractionLimit();

}

int LevelManager::GetEnemyChoice(int enemyIndex) {

	return allEnemies[enemyIndex]->InteractionChoice();

}

void LevelManager::ClearEnemy(int enemyIndex) {

	allEnemies[enemyIndex]->SetState();

}

int LevelManager::ManageEnemies(Play::Point2D playerPos, float elapsedTime) {
	bool enemyMoved = false;
	secondsTimer += elapsedTime;
	if (allEnemies.size() > 0) {

		for (int i = 0; i < allEnemies.size(); i++) {

			if (secondsTimer > 1.0f) {
				while (enemyMoved == false) {
					if (allEnemies[i]->HandleControls(allEnemies[i]->GetRandom(4)) == true) {
						break;
					}
				}
			}
			allEnemies[i]->DrawCharacter();
			if (allEnemies[i]->GetPosition() == playerPos) {

				if (allEnemies[i]->GetState() == false) {

					return i;

				}
				
			}

		}
		if (secondsTimer > 1.0f) {
			secondsTimer = 0.0f;
		}
	}
	return -1;

}

void LevelManager::SetLevel(float newLevel) {

	levelIndex = newLevel;

}

void LevelManager::TrapLevel(int increase) {

	levelIndex += increase;

}

void LevelManager::SetLevel() {

	if (levelIndex > 6) {
		levelIndex -= 10;
	}

	levelIndex++;

}

void LevelManager::SetMode(int newDifficulty) {

	difficulty = newDifficulty;

}

void LevelManager::LoadLevel() {

	std::string newLevel = "(ID)[" + std::to_string(levelIndex) + "]";
	std::string newMode = "(Mode)[" + std::to_string(difficulty) + "]";
	std::ifstream inFile(TextFile);
	std::string newLine;
	bool levelFound = false;
	bool modeFound = false;

	if (!inFile.is_open()) {

		return;

	}
	else {

		// This loop reads and relays a specified level.
		while (std::getline(inFile, newLine)) {

			if (newLine.rfind(newMode, 0) == 0) {

				modeFound = true;

			}
			if (newLine.rfind(newLevel, 0) == 0) {

				levelFound = true;

			}

			if (modeFound && levelFound) {

				if (newLine.rfind("(Tile)", 0) == 0) {

					if (isNew == false) {

						levelTiles.clear();
						trapPos.clear();
						enemyPos.clear();
						openTiles.clear();
						isNew = true;

					}

					levelTiles.push_back(newLine.substr(6));

				}

				else if (newLine == "[End]") {

					modeFound = false;
					levelFound = false;
					break;

				}

			}

		}

	}
	
	inFile.close();

}

void LevelManager::PrintLevel() {

	std::string str;
	int tempInt;
	if (levelTiles[0] == "MAX") {
		startPos = { (DISPLAY_WIDTH * 0.5f) - DISPLAY_TILE, 0 };
		endPos = { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) - DISPLAY_TILE };
		for (int i = 0; i < DISPLAY_HEIGHT / DISPLAY_TILE; i++) {

			for (int j = 0; j < DISPLAY_WIDTH / DISPLAY_TILE; j++) {

				if (i < (DISPLAY_HEIGHT / DISPLAY_TILE) * 0.5) {
					Play::DrawRect({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);
				}
				else {
					Play::DrawRect({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGrey, true);
				}
				openTiles.push_back({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) });
			}

		}
	}
	else {
		for (int i = 0; i < levelTiles.size(); i++) {

			str = levelTiles[i];
			for (int j = 0; j < str.length(); j++) {
				tempInt = (((DISPLAY_WIDTH / DISPLAY_TILE) - str.length()) * 0.5);
				if (str.at(j) == '1') {

					//wall
					Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGrey, true);

				}
				else {

					if (str.at(j) == 'Y') {

						//exit
						endPos = { (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) };
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGreen, true);

					}
					else if (str.at(j) == 'X') {

						//entrance
						startPos = { (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) };
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cBlue, true);

					}
					else if (str.at(j) == 'E') {

						//location of an enemy
						enemyPos.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);

					}
					else if (str.at(j) == 'K') {

						//location of a key
						keyPos.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cMagenta, true);

					}
					else if (str.at(j) == 'T') {

						//location of a trap
						trapPos.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cMagenta, true);

					}
					else if (str.at(j) == '0') {

						//empty space
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);

					}

					openTiles.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
					tempInt++;

				}

			}

		}

	}
	for (int z = DISPLAY_TILE; z < DISPLAY_WIDTH; z += DISPLAY_TILE) {

		Play::DrawLine({ z, 0 }, { z, DISPLAY_HEIGHT }, Play::cBlack);
		Play::DrawLine({ 0, z }, { DISPLAY_WIDTH, z }, Play::cBlack);

	}

	isNew = false;

}

std::vector<Play::Point2D> LevelManager::GetLevel() {

	Play::ClearDrawingBuffer(Play::cGrey);

	LoadLevel();
	PrintLevel();

	return std::vector<Play::Point2D> { startPos, endPos };

}