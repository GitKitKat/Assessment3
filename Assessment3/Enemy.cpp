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

}

Enemy::~Enemy() {



}

int Enemy::GetDirection() {
	// Get a new random number
	srand(time(0));

	// Return a random number between 0 and 3 (including)
	return rand() % 4;

}

Play::Point2D Enemy::GetPosition() {

	return characterPos;

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