/* Includes: 
* This file's header */
#include "LevelManager.h"

LevelManager::LevelManager() {
	/* Definitions */
	// 
	levelIndex = 0;
	difficulty = 1; // starting difficulty is EASY
	secondsTimer = 0.0f;
	startPos = { 0.0f, 0.0f };
	endPos = { 0.0f, 0.0f };
	encounter = false; 
	clearedEnemies = 0;
	clearedLevel = 0;
	defeatedEnemies = false;
	textFile = "Data\\Textfiles\\Levels.txt";

	// Loads the current level from the textFile
	LoadLevel();

}

LevelManager::~LevelManager() {
	// Destructor
}

int LevelManager::GetRandom(int numLimit) {
	// Get a new random number
	srand(time(0));

	// Return a random number between 0 and the limit given
	return int(rand() % numLimit);

}

void LevelManager::SetMode(int newDifficulty) {
	difficulty = newDifficulty;
}

int LevelManager::GetLevelIndex() {
	return levelIndex;
}

std::vector<Play::Point2D> LevelManager::GetLevel() {

	Play::ClearDrawingBuffer(Play::cGrey); // Clears the game window
	PrintLevel();
	return std::vector<Play::Point2D> { startPos, endPos };

}

std::map<float, std::vector<Play::Point2D>> LevelManager::GetBoundaries() {
	return openTiles;
}

void LevelManager::SetLevel(float newLevel) {

	Play::PlayAudio("level");
	levelIndex = newLevel;
	LoadLevel();

}

void LevelManager::SetLevel() {
	
	Play::PlayAudio("level");
	if (levelIndex > 10) { // If the current level was trapped, sends the player to the next level in the original sequence
		levelIndex -= 10;
	}
	levelIndex++;
	LoadLevel();

}

bool LevelManager::CheckTraps(Play::Point2D playerPos) {

	if (trapPos.size() > 0) { // Checks if the current level has a trap
		for (int i = 0; i < trapPos.size(); i++) {
			if (trapPos[i] == playerPos) { // If the player is standing on a trap, changes the level layout

				TrapLevel(10);
				return true;

			}
		}
	}
	if (keyPos.size() > 0) { // Checks if the current level has a key
		for (int i = 0; i < keyPos.size(); i++) {
			if (keyPos[i] == playerPos) { // If the player is standing on a key, reverts the level layout

				TrapLevel(-10);
				return true;

			}
		}
	}
	return false;

}

void LevelManager::TrapLevel(int increase) {

	Play::PlayAudio("trap");
	levelIndex += increase;
	LoadLevel();

}

void LevelManager::LoadLevel() {

	levelTiles.clear(); // Clears the previously loaded level
	// Stores the ID of the level to be loaded
	std::string newLevel = "(ID)[" + std::to_string(levelIndex) + "]";
	// Stores the difficulty mode of the level to be loaded
	std::string newMode = "(Mode)[" + std::to_string(difficulty) + "]";
	// Opens the text file for reading
	std::ifstream inFile(textFile);
	// Stores each line of the opened file as a new string
	std::string newLine;
	// Whether the level was successfully found
	bool levelFound = false;
	// Whether a level that matches the current difficulty mode was found
	bool modeFound = false;

	if (!inFile.is_open()) { // Returns if the text file could not be opened
		return;
	}
	else {
		// This loop reads and relays the specified level
		while (std::getline(inFile, newLine)) {

			if (newLine.rfind(newMode, 0) == 0) { // Checks if levels with the current difficulty were found
				modeFound = true;
			}

			if (modeFound && newLine.rfind(newLevel, 0) == 0) { // Checks if the specified level was found
				levelFound = true;
			}

			if (modeFound && levelFound) { 

				if (newLine.rfind("(Tile)", 0) == 0) {
					levelTiles.push_back(newLine.substr(6)); // Stores each horizontal line of the level as a separate string
				}
				else if (newLine == "[End]") { // Ends the reading

					modeFound = false;
					levelFound = false;
					break;

				}

			}

		}

	}
	inFile.close(); // Closes the file

}

void LevelManager::PrintLevel() {
	// Clears the existing vectors of the previous level's data
	trapPos.clear();
	keyPos.clear();
	enemyPos.clear();
	openTiles.clear();
	// Stores each horizontal line of the loaded level as a string
	std::string tempStr;
	// Used to determine which tile a level should be printed from in order to horizontally center it in the game window
	int tempInt;
	// Stores the positions of all passable tiles in a horizontal line 
	std::vector<Play::Point2D> tempVector;
	if (levelTiles[0] == "MAX") { 
		startPos = { (DISPLAY_WIDTH * 0.5f) - DISPLAY_TILE, 0 };
		endPos = { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) - DISPLAY_TILE };
		for (int i = 0; i < DISPLAY_HEIGHT / DISPLAY_TILE; i++) { // i represents a vertical (number of tiles up from zero)

			for (int j = 0; j < DISPLAY_WIDTH / DISPLAY_TILE; j++) { // j represents a horizontal (number of tiles to the right from zero)

				if (i < (DISPLAY_HEIGHT / DISPLAY_TILE) * 0.5) { // Draws the bottom half of the game window as passable tiles
					Play::DrawRect({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);
					tempVector.push_back({ float(DISPLAY_TILE * j), float(DISPLAY_TILE * i) });
				}
				else { // Draws the top half of the game window as impassable tiles (walls)
					Play::DrawRect({ (DISPLAY_TILE * j), (DISPLAY_TILE * i) }, { DISPLAY_TILE + (DISPLAY_TILE * j), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGrey, true);
				}
			}
			if (tempVector.size() > 0) { // Stores all the passable tiles within a horizontal line and the corresponding line
				openTiles.insert({ i, tempVector }); 
				tempVector.clear();
			}
		}
	}
	else {
		for (int i = 0; i < levelTiles.size(); i++) { // i represents a vertical (number of tiles up from zero)
			tempStr = levelTiles[i];
			for (int j = 0; j < tempStr.length(); j++) { // j represents a horizontal (number of tiles to the right from zero)
				tempInt = (((DISPLAY_WIDTH / DISPLAY_TILE) - tempStr.length()) * 0.5);

				if (tempStr.at(j) == 'R') { // Randomly generates a wall or open tile
					// Stores the generated tile. 1 corresponds to a wall, 0 corresponds to a passable tile
					tempStr.at(j) = *std::to_string(GetRandom(2)).c_str();
				}
				if (tempStr.at(j) == '1') { // Draws a wall on the game window
					Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGrey, true);
				}
				else {

					if (tempStr.at(j) == 'Y') { // Draws the exit of a level
						
						endPos = { (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) };
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cGreen, true);

					}
					else if (tempStr.at(j) == 'X') { // Draws the entrance of a level
						
						startPos = { (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) };
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);

					}
					else if (tempStr.at(j) == 'E') { // Stores and draws the location of an enemy
						
						enemyPos.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);

					}
					else if (tempStr.at(j) == 'K') { // Stores and draws the location of a key

						keyPos.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cMagenta, true);

					}
					else if (tempStr.at(j) == 'T') { // Stores and draws the location of a key

						trapPos.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cMagenta, true);

					}
					else if (tempStr.at(j) == '0') { // Draws a passable tile on the game window
						Play::DrawRect({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) }, { (DISPLAY_TILE * j) + ((tempInt + 1) * DISPLAY_TILE), DISPLAY_TILE + (DISPLAY_TILE * i) }, Play::cYellow, true);
					}
					// Stores all the passable tiles in a line
					tempVector.push_back({ (DISPLAY_TILE * j) + (tempInt * DISPLAY_TILE), (DISPLAY_TILE * i) });
					tempInt++;

				}

			}
			if (tempVector.size() > 0) { // Stores all the passable tiles within a horizontal line and the corresponding line
				openTiles.insert({ i, tempVector });
				tempVector.clear();
			}
		}

	}
	for (int z = DISPLAY_TILE; z < DISPLAY_WIDTH; z += DISPLAY_TILE) { // Draws square gridlines in the game window

		Play::DrawLine({ z, 0 }, { z, DISPLAY_HEIGHT }, Play::cBlack);
		Play::DrawLine({ 0, z }, { DISPLAY_WIDTH, z }, Play::cBlack);

	}

}

void LevelManager::CreateEnemies() {

	if (allEnemies.size() > 0 && allEnemies.size() != enemyPos.size()) { // Resets the vector if the number of enemy instances doesn't match the number of starting positions
		DeleteEnemies();
	}

	if (enemyPos.size() > 0 && allEnemies.size() == 0) { // Creates and stores pointers to instances of the enemy class 
		for (int i = 0; i < enemyPos.size(); i++) {
			// A pointer to an instance of the enemy class
			Enemy* roamingEnemy = new Enemy();
			roamingEnemy->SetBoundaries(GetBoundaries());
			roamingEnemy->SetPosition(enemyPos[i]);
			roamingEnemy->SetEncounter(difficulty);
			allEnemies.push_back(roamingEnemy);

		}
	}
	else if (enemyPos.size() > 0 && allEnemies.size() > 0) { // Resets the encounters for all enemies
		for (int i = 0; i < enemyPos.size(); i++) {
			allEnemies[i]->SetEncounter(difficulty);
		}
	}

}

int LevelManager::ManageEnemies(Play::Point2D playerPos, float elapsedTime) {
	secondsTimer += elapsedTime;
	if (allEnemies.size() > 0) {

		for (int i = 0; i < allEnemies.size(); i++) {

			if (secondsTimer > 1.0f) { // Limits enemy movement to once per second
				allEnemies[i]->HandleControls(allEnemies[i]->GetRandom(4)); // Randomizes the direction the enemy moves in
			}
			allEnemies[i]->DrawCharacter();
			if (allEnemies[i]->GetPosition() == playerPos) { // Checks if an enemy has encountered the player

				if (!allEnemies[i]->GetState()) { // Returns the encountered enemy's index if it hasn't been cleared
					return i;
				}
				
			}

		}
		if (secondsTimer > 1.0f) { // Only resets the timer if all enemies have moved 
			secondsTimer = 0.0f;
		}
	}

	return -1;

}

std::vector<std::string> LevelManager::GetEnemyEncounter(int enemyIndex) {
	return allEnemies[enemyIndex]->GetEncounter();
}

std::vector<std::string> LevelManager::GetEncounterChoices(int enemyIndex) {
	return allEnemies[enemyIndex]->GetEncounterChoices();
}

int LevelManager::GetEnemyChoice(int enemyIndex) {
	return allEnemies[enemyIndex]->GetEnemyChoice();
}

bool LevelManager::GetLeaderlessClear() {

	if (clearedLevel >= 7) { 
		return true;
	}
	else {
		return false;
	}

}

bool LevelManager::GetPacifistClear() {
	return !defeatedEnemies; // Returns false if any enemies were defeated (i.e. defeatedEnemies = true)
}

void LevelManager::ClearEnemy(int enemyIndex) {

	allEnemies[enemyIndex]->SetState(); // Changes the enemy's state
	clearedEnemies++;
	defeatedEnemies = true;

}

void LevelManager::DeleteEnemies() {

	if (clearedEnemies == allEnemies.size()) { // Check if the number of cleared Enemies matches the number of created enemies
		clearedLevel++;
		clearedEnemies = 0;
	}
	for (int i = 0; i < allEnemies.size(); i++) { // Deletes pointers to the enemy class before clearing the vector
		delete allEnemies[i];
	}
	allEnemies.clear();

}
