/* Includes: 
This file's header */
#include "Enemy.h"

/* Standard header files */
#include <cstdlib>

Enemy::Enemy() {

	characterPos = { 0.0f, 0.0f };
	posIncrease = 1.0f;
	newPos = { 0.0f, 0.0f };
	enemyCleared = false;
	enemyMovement = false;
	itemsFile = "Data\\Textfiles\\ItemList.txt";

}

Enemy::~Enemy() {



}

int Enemy::GetRandom(int numLimit) {
	// Get a new random number
	srand(time(0));

	// Return a random number between 0 and the limit given
	return int(rand() % numLimit);

}

Play::Point2D Enemy::GetPosition() {

	return characterPos;

}

void Enemy::SetState() {

	enemyCleared = true;


}

bool Enemy::GetState() {

	return enemyCleared;

}

void Enemy::LoadInteraction(int difficulty, std::string interactionIndex) {

	std::string newInteraction = "(ID)[" + interactionIndex + "]";
	std::string newMode = "(Mode)[" + std::to_string(difficulty) + "]";
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

			if (newLine.rfind(newMode, 0) == 0) {

				modeFound = true;

			}
			if (newLine.rfind(newInteraction, 0) == 0) {

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

	std::string i = std::to_string(GetRandom(4) + 1);
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

void Enemy::CheckCollision() {

	bool colliderCheck = true;
	float secondEdge = DISPLAY_TILE * posIncrease;
	auto itr = tempBoundaries.begin();
	int z;
	if (itr->first == 0) {

		z = int(tempBoundaries.size());

	}
	else {

		z = int(tempBoundaries.size() + itr->first);

	}

	for (float i = itr->first; i < z; i++) {

		if (newPos.y <= (DISPLAY_TILE * i) && newPos.y > (DISPLAY_TILE * i) - secondEdge) {

			for (Play::Point2D j : tempBoundaries[i]) {

				if (newPos.x >= j.x && newPos.x < j.x + secondEdge) {

					colliderCheck = false;

				}

			}

		}

	}

	if (!colliderCheck) {

		enemyMovement = true;
		SetPosition(newPos);

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