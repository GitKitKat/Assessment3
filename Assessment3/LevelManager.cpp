#include "LevelManager.h"

#include "Play.h"

LevelManager::LevelManager() {

	levelIndex = 1;
	difficulty = 1;
	//levelTiles.push_back("");
	startPos = { 0.0f, 0.0f };
	endPos = { 0.0f, 0.0f };
	//LoadLevel();

}

LevelManager::~LevelManager() {



}

std::vector<Play::Point2D> LevelManager::GetBoundaries() {

	std::vector<Play::Point2D> tempVector = openTiles;
	openTiles.clear();

	return tempVector;

}

void LevelManager::CreateObstacles() {

	if (trapPos.size() > 0) {



	}

	if (enemyPos.size() > 0) {

		for (int i = 0; i < enemyPos.size(); i++) {

			levelEnemy->SetPositions(enemyPos);

		}

	}

}

void LevelManager::SetLevel(int newLevel) {

	levelIndex = newLevel;

}

void LevelManager::SetLevel() {

	levelIndex++;

}

void LevelManager::SetMode(int newDifficulty) {

	difficulty = newDifficulty;

}

void LevelManager::LoadLevel() {

	std::string newLevel = "(ID)[" + std::to_string(levelIndex) + "]";
	std::string newMode = "(ID)[" + std::to_string(difficulty) + "]";
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

	for (int i = 0; i < levelTiles.size(); i++) {

		str = levelTiles[i];

		for (int j = 0; j < str.length(); j++) {

			if (str.at(j) == '1') {

				//wall
				Play::DrawRect({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGrey, true);

			}
			else {

				if (str.at(j) == 'Y') {

					//exit
					endPos = { float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) };
					Play::DrawRect({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGreen, true);

				}
				else if (str.at(j) == 'X') {

					//entrance
					startPos = { float(DISPLAY_TILE * j), float(DISPLAY_TILE * (i)) };
					Play::DrawRect({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cBlue, true);

				}
				else if (str.at(j) == 'E') {

					//location of an enemy
					enemyPos.push_back({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * (i)) });
					Play::DrawRect({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cRed, true);

				}
				else if (str.at(j) == 'T') {

					//location of a trap
					trapPos.push_back({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * (i)) });
					Play::DrawRect({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGreen, true);

				}
				else if (str.at(j) == '0') {

					//empty space
					Play::DrawRect({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);

				}
				
				openTiles.push_back({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) });

			}

		}

	}

	for (int z = DISPLAY_TILE; z < DISPLAY_WIDTH; z += DISPLAY_TILE) {

		Play::DrawLine({ z, 0 }, { z, DISPLAY_HEIGHT }, Play::cGrey);
		Play::DrawLine({ 0, z }, { DISPLAY_WIDTH, z }, Play::cGrey);

	}

	isNew = false;

}

std::vector<Play::Point2D> LevelManager::GetLevel() {

	Play::ClearDrawingBuffer(Play::cBlack);

	LoadLevel();
	PrintLevel();

	std::vector<Play::Point2D> levelExits = { startPos, endPos };

	return levelExits;

}