/* Includes: 
This file's header */
#include "Enemy.h"

/* Standard header files */
#include <cstdlib>

Enemy::Enemy() {
	/* Definitions: */
	// 
	characterPos = { 0.0f, 0.0f };
	posIncrease = 1.0f;
	newPos = { 0.0f, 0.0f };
	enemyCleared = false;
	enemyMovement = false;
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

void Enemy::SetState() {

	enemyCleared = true;


}

bool Enemy::GetState() {

	return enemyCleared;

}

void Enemy::LoadInteraction(int difficulty, std::string interactionIndex) {

	std::string newInteraction = "(ID)[" + interactionIndex + "]";
	std::ifstream inFile(itemsFile);
	std::string newLine;
	bool interactionFound = false;
	bool modeFound = false;

	if (!inFile.is_open()) {

		return;

	}
	else {

		// This loop reads and relays a specified interaction.
		while (std::getline(inFile, newLine)) {

			if (newLine.rfind("(Mode)", 0) == 0) {
				std::string newMode = newLine.substr(7);
				newMode.pop_back();
				if (difficulty >= stoi(newMode)) {

					modeFound = true;

				}
			}
			if (modeFound == true && newLine.rfind(newInteraction, 0) == 0) {

				interactionFound = true;

			}

			if (modeFound && interactionFound) {

				if (newLine.rfind("(Name)", 0) == 0) {

					enemyInteraction.push_back(newLine.substr(6));
					
				}
				else if (newLine.rfind("(Choice)", 0) == 0) {

					interactionLimit.push_back(newLine.substr(8));

				}
				else if (newLine.rfind("(Desc)", 0) == 0) {

					enemyInteraction.push_back(newLine.substr(6));

				}

				else if (newLine == "[End]") {

					modeFound = false;
					interactionFound = false;
					break;

				}

			}

		}

	}

	inFile.close();

}

std::vector<std::string> Enemy::GetInteraction() {

	return enemyInteraction;

}

int Enemy::InteractionChoice() {

	return GetRandom(interactionLimit.size());

}

std::vector<std::string> Enemy::InteractionLimit() {

	return interactionLimit;

}

void Enemy::SetInteraction(int difficulty) {

	std::string i = std::to_string(GetRandom(difficulty + 1) + 1);
	enemyInteraction.clear();
	interactionLimit.clear();
	LoadInteraction(difficulty, i);

}

void Enemy::DrawCharacter() {

	if (!enemyCleared) {

		Play::DrawRect(characterPos, { characterPos.x + float(DISPLAY_TILE), characterPos.y + float(DISPLAY_TILE) }, enemyColours[0], true);

	}
	else {

		Play::DrawRect(characterPos, { characterPos.x + float(DISPLAY_TILE), characterPos.y + float(DISPLAY_TILE) }, enemyColours[1], true);

	}

}

bool Enemy::HandleControls(int randomNum) {
	// Enemy moves up
	if (randomNum == 1) {

		newPos = { characterPos.x, characterPos.y + (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	// Enemy moves down
	else if (randomNum == 3) {

		newPos = { characterPos.x, characterPos.y - (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	// Enemy moves to the left
	else if (randomNum == 0) {

		newPos = { characterPos.x - (posIncrease * DISPLAY_TILE), characterPos.y };
		CheckCollision();

	}
	// Enemy moves to the right
	else if (randomNum == 2) {

		newPos = { characterPos.x + (posIncrease * DISPLAY_TILE), characterPos.y };
		CheckCollision();

	}

	return enemyMovement;

}