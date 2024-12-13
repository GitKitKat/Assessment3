#include "Player.h"

Player::Player() {

	playerPos = { 0.0f, 0.0f };
	posIncrease = 0.0f;
	newPos = { 0.0f, 0.0f };

}

Player::~Player() {



}

void Player::CheckCollision() {

	bool colliderCheck = true;
	float secondEdge = DISPLAY_TILE * posIncrease;
	auto itr = tempBoundaries.begin();
	float z;
	if (itr->first == 0) {

		z = tempBoundaries.size();

	}
	else {

		z = tempBoundaries.size() + itr->first;

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

		SetPosition(newPos);

	}

}

void Player::SetBoundaries(std::vector<Play::Point2D> levelBounds) {

	std::vector<Play::Point2D> tempVector;
	tempBoundaries.clear();

	for (int i = 0; i < levelBounds.size(); i++) {

		for (int j = 0; j < levelBounds.size(); j++) {

			if (levelBounds[j].y <= (DISPLAY_TILE * i) && levelBounds[j].y > (DISPLAY_TILE * i) - DISPLAY_TILE) {

				tempVector.push_back(levelBounds[j]);

			}

		}

		if (tempVector.size() > 0) {

			tempBoundaries.insert({ i, tempVector });

		}

		tempVector.clear();

	}
}

void Player::SetPosition(Play::Point2D userPos) {

	if (userPos.y < 0) {
		userPos.y = 0;
	}
	playerPos = userPos;

}

void Player::SetPosition(std::vector<Play::Point2D> levelExits) {

	SetPosition(levelExits[0]);
	//levelExits[1].y -= DISPLAY_TILE;
	endPos = levelExits[1];

	//tempBoundaries.clear();

}

void Player::SetColour(std::vector<Play::Colour> colourList, int newColour) {

	playerColour[0] = colourList[newColour];

}

void Player::DrawPlayer() {

	Play::DrawRect(playerPos, { playerPos.x + float(DISPLAY_TILE), playerPos.y + float(DISPLAY_TILE) }, playerColour[0], true);
	for (int i = 3; i > 0; i--) {

		Play::DrawCircle({ playerPos.x + float(DISPLAY_TILE * 0.5), playerPos.y + float(DISPLAY_TILE * 0.5) }, i, Play::cBlack);

	}
	

}

bool Player::HandleControls(std::vector<Play::KeyboardButton> gameControls) {

	if (Play::KeyDown(Play::KEY_SHIFT)) {

		posIncrease = 0.5f;

	}
	else {

		posIncrease = 1.0f;

	}

	if (Play::KeyPressed(gameControls[1])) {

		newPos = { playerPos.x, playerPos.y + (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	else if (Play::KeyPressed(gameControls[3])) {

		newPos = { playerPos.x, playerPos.y - (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	else if (Play::KeyPressed(gameControls[0])) {

		newPos = { playerPos.x - (posIncrease * DISPLAY_TILE), playerPos.y };
		CheckCollision();

	}
	else if (Play::KeyPressed(gameControls[2])) {

		newPos = { playerPos.x + (posIncrease * DISPLAY_TILE), playerPos.y };
		CheckCollision();

	}

	if (endPos == playerPos) {

		return true;

	}
	else {

		return false;

	}

}