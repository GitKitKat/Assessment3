#include "LevelManager.h"

#include "Play.h"

LevelManager::LevelManager() {

	levelIndex = "1";
	difficulty = 1;
	levelTiles.push_back("");
	startPos = { 0.0f, 0.0f };

}

LevelManager::~LevelManager() {



}

void LevelManager::SetLevel(int newLevel) {

	levelIndex = std::to_string(newLevel);

}

void LevelManager::SetMode(int newDifficulty) {

	difficulty = newDifficulty;

}

void LevelManager::LoadLevel() {

	std::string newLevel = "(ID)[" + levelIndex + "]";
	std::ifstream inFile(TextFile);

	if (!inFile.is_open()) {

		// This is printed if the file loading fails.
		//std::cout << "The text adventure, " << TextFile << ", couldn't be opened." << std::endl;
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

			if (str.at(j) == 'Y') {

				//exit
				Play::DrawRect({ 0 + (DISPLAY_TILE * j), 0 + (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGreen, true);

			}
			else if (str.at(j) == 'X') {

				//entrance
				startPos = { float((0 + (DISPLAY_TILE * j))), float((0 + (DISPLAY_TILE * i))) };
				Play::DrawRect({ 0 + (DISPLAY_TILE * j), 0 + (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cBlue, true);

			}
			else if (str.at(j) == '0') {

				//empty space
				Play::DrawRect({ 0 + (DISPLAY_TILE * j), 0 + (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);

			}
			else if (str.at(j) == '1') {

				//wall
				Play::DrawRect({ 0 + (DISPLAY_TILE * j), 0 + (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGrey, true);
				//Play::DrawRect({ (DISPLAY_WIDTH / 2) + 150, (DISPLAY_HEIGHT / 2) - 45 }, { (DISPLAY_WIDTH / 2) + 180, (DISPLAY_HEIGHT / 2) - 15 }, Play::cBlue, true);

			}

		}

	}

	for (int z = DISPLAY_TILE; z < DISPLAY_WIDTH; z += DISPLAY_TILE) {

		//Play::DrawLine(int startX, int startY, int endX, int endY, Pixel pix);
		Play::DrawLine({ z, 0 }, { z, DISPLAY_HEIGHT }, Play::cGrey);
		Play::DrawLine({ 0, z }, { DISPLAY_WIDTH, z }, Play::cGrey);

	}

	isNew = false;

}

Play::Point2D LevelManager::GetLevel() {

	Play::ClearDrawingBuffer(Play::cBlack);

	LoadLevel();
	PrintLevel();

	return startPos;

}