/* Includes: 
This file's header */
#include "Enemy.h"

/* Standard header files */
#include <cstdlib>

Enemy::Enemy() {
	/* Definitions: */
	// 
	creaturePos = { 0.0f, 0.0f };
	newPos = { 0.0f, 0.0f };
	posIncrease = 1.0f; // default movement is one tile at a time
	enemyCleared = false;
	itemsFile = "Data\\Textfiles\\ItemList.txt";

}

Enemy::~Enemy() {
	// Destructor
}

int Enemy::GetRandom(int numLimit) {
	// Get a new random number
	srand(time(0));

	// Return a random number between 0 and the limit given
	return int(rand() % numLimit);

}

bool Enemy::GetState() {
	return enemyCleared;
}

std::vector<std::string> Enemy::GetEncounter() {
	return enemyEncounter;
}

std::vector<std::string> Enemy::GetEncounterChoices() {
	return encounterChoices;
}

int Enemy::GetEnemyChoice() {
	return GetRandom(encounterChoices.size());
}

void Enemy::SetState() {
	enemyCleared = true;
}

void Enemy::SetEncounter(int difficulty) {

	enemyEncounter.clear();
	encounterChoices.clear();
	LoadEncounter(difficulty, GetRandom(difficulty + 1) + 1); // There are two encounters available in EASY mode, three in NORMAL, and four in HARD

}

void Enemy::LoadEncounter(int difficulty, int encounterIndex) {
	// Stores the ID of the encounter to be loaded
	std::string newEncounter = "(ID)[" + std::to_string(encounterIndex) + "]";
	// Opens the file for reading
	std::ifstream inFile(itemsFile);
	// Stores each line of the opened file as a new string
	std::string newLine;
	// Whether the encounter was successfully found
	bool encounterFound = false;
	// Whether an encounter that matches the current difficulty mode was found
	bool modeFound = false;

	if (!inFile.is_open()) { // Returns if the file could not be opened
		return;
	}
	else {
		// This loop reads and relays the specified encounter
		while (std::getline(inFile, newLine)) {

			if (newLine.rfind("(Mode)", 0) == 0) { // Checks whether this encounter falls under the current difficulty
				// Stores the difficulty of the currently read encounter
				std::string newMode = newLine.substr(7);
				newMode.pop_back();
				if (difficulty >= stoi(newMode)) { // Each difficulty has access to all encounters in the previous mode (i.e. HARD has all of NORMAL's encounters as options) 
					modeFound = true;
				}
			}

			if (modeFound == true && newLine.rfind(newEncounter, 0) == 0) { // After the difficulty is matched, searches for the specified encounter by ID
				encounterFound = true;
			}

			if (modeFound && encounterFound) {

				if (newLine.rfind("(Name)", 0) == 0) { // Stores the name of the encounter
					enemyEncounter.push_back(newLine.substr(6));
				}
				else if (newLine.rfind("(Choice)", 0) == 0) { // Stores each available choice
					encounterChoices.push_back(newLine.substr(8));
				}
				else if (newLine.rfind("(Desc)", 0) == 0) { // Stores the description of the encounter
					enemyEncounter.push_back(newLine.substr(6));
				}
				else if (newLine == "[End]") { // Ends the reading

					modeFound = false;
					encounterFound = false;
					break;

				}

			}

		}

	}
	inFile.close(); // Closes the file

}

void Enemy::HandleControls(int randomNum) {
	// Enemy moves up
	if (randomNum == 1) {

		newPos = { creaturePos.x, creaturePos.y + (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	// Enemy moves down
	else if (randomNum == 3) {

		newPos = { creaturePos.x, creaturePos.y - (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	// Enemy moves to the left
	else if (randomNum == 0) {

		newPos = { creaturePos.x - (posIncrease * DISPLAY_TILE), creaturePos.y };
		CheckCollision();

	}
	// Enemy moves to the right
	else if (randomNum == 2) {

		newPos = { creaturePos.x + (posIncrease * DISPLAY_TILE), creaturePos.y };
		CheckCollision();

	}

}

void Enemy::DrawCharacter() {
	// The enemy is drawn red if uncleared; Green if cleared
	if (!enemyCleared) {
		Play::DrawRect(creaturePos, { creaturePos.x + float(DISPLAY_TILE), creaturePos.y + float(DISPLAY_TILE) }, Play::cRed, true);
	}
	else {
		Play::DrawRect(creaturePos, { creaturePos.x + float(DISPLAY_TILE), creaturePos.y + float(DISPLAY_TILE) }, Play::cGreen, true);
	}

}