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

	for (int i = 0; i < tempBoundaries.size(); i++) {

		if (newPos.y <= (DISPLAY_TILE * i) && newPos.y > (DISPLAY_TILE * i) - DISPLAY_TILE) {

			for (Play::Point2D j : tempBoundaries[i]) {

				if (newPos.x >= j.x && newPos.x < j.x + DISPLAY_TILE) {

					colliderCheck = false;

				}

			}

		}

	}

	if (!colliderCheck) {

		SetPosition(newPos);

	}

}

void Player::SetBoundaries(int levelLength, std::vector<Play::Point2D> levelBounds) {

	levelLength = levelBounds.size();
	std::vector<Play::Point2D> tempVector;

	for (int i = 0; i < levelLength; i++) {

		for (int j = 0; j < levelLength; j++) {

			if (levelBounds[j].y <= (DISPLAY_TILE * i) + DISPLAY_TILE && levelBounds[j].y > (DISPLAY_TILE * i)) {

				levelBounds[j].y -= DISPLAY_TILE;
				tempVector.push_back(levelBounds[j]);

			}

		}

		tempBoundaries.insert({ i, tempVector });
		tempVector.clear();

	}
}

void Player::SetPosition(Play::Point2D userPos) {

	playerPos = userPos;

}

void Player::SetPosition(std::vector<Play::Point2D> levelExits) {

	playerPos = levelExits[0];
	endPos = levelExits[1];

}

void Player::SetColour(Play::Colour arr[], int newColour) {

	playerColour[0] = arr[newColour];

}

void Player::DrawPlayer() {

	Play::DrawRect(playerPos, { playerPos.x + float(DISPLAY_TILE), playerPos.y + float(DISPLAY_TILE) }, playerColour[0], true);
	for (int i = 10; i > 0; i--) {

		Play::DrawCircle({ playerPos.x + float(DISPLAY_TILE * 0.5), playerPos.y + float(DISPLAY_TILE * 0.5) }, i, Play::cBlack);

	}
	

}

void Player::HandleControls() {

	if (Play::KeyDown(Play::KEY_SHIFT)) {

		posIncrease = 0.5f;

	}
	else {

		posIncrease = 1.0f;

	}

	if (Play::KeyPressed(Play::KEY_UP)) {

		newPos = { playerPos.x, playerPos.y + (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	else if (Play::KeyPressed(Play::KEY_DOWN)) {

		newPos = { playerPos.x, playerPos.y - (posIncrease * DISPLAY_TILE) };
		CheckCollision();

	}
	else if (Play::KeyPressed(Play::KEY_LEFT)) {

		newPos = { playerPos.x - (posIncrease * DISPLAY_TILE), playerPos.y };
		CheckCollision();

	}
	else if (Play::KeyPressed(Play::KEY_RIGHT)) {

		newPos = { playerPos.x + (posIncrease * DISPLAY_TILE), playerPos.y };
		CheckCollision();

	}

	DrawPlayer();

}