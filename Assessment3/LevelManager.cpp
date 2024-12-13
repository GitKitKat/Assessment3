#include "LevelManager.h"

#include "Play.h"

LevelManager::LevelManager() {

	levelIndex = 1;
	difficulty = 1;
	levelTiles.push_back("");
	startPos = { 0.0f, 0.0f };
	endPos = { 0.0f, 0.0f };

}

LevelManager::~LevelManager() {



}

std::vector<Play::Point2D> LevelManager::GetBoundaries() {

	std::vector<Play::Point2D> tempVector = openTiles;
	openTiles.clear();

	return tempVector;

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
	std::ifstream inFile(TextFile);
	std::string newLine;

	if (!inFile.is_open()) {

		return;

	}
	else {

		// This loop reads and relays a specified level.
		while (std::getline(inFile, newLine)) {

			
			if (newLine.rfind(newLevel, 0) == 0) {

				levelFound = true;

			}

			else if (newLine.rfind("(Tile)", 0) == 0 && levelFound == true) {

				if (isNew == false) {

					levelTiles.clear();
					isNew = true;

				}

				levelTiles.push_back(newLine.substr(6));

			}

			// Each scene in the text file ends with the [EndScene] line. 
			// Here, that scene data is passed to the myAdventure object and the pointer is emptied.
			else if (newLine == "[End]" && levelFound == true) {

				levelFound = false;
				break;

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
					startPos = { float(DISPLAY_TILE * j), float(DISPLAY_TILE * (i - 1)) };
					Play::DrawRect({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cBlue, true);

				}
				else if (str.at(j) == '0') {

					//empty space
					Play::DrawRect({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);

				}
				
				openTiles.push_back({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) });

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