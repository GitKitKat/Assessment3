/* Includes: 
This file's header */
#include "Player.h"

Player::Player() {

	characterPos = { 0.0f, 0.0f };
	posIncrease = 0.0f;
	newPos = { 0.0f, 0.0f };
	playerDirection = { 0.0f, 0.0f };

}

Player::~Player() {



}

Play::Point2D Player::GetPosition() {

	return characterPos;

}

void Player::CheckCollision() {

	bool colliderCheck = true;
	float secondEdge = DISPLAY_TILE * posIncrease;
	auto itr = tempBoundaries.begin();
	float z;
	if (itr->first == 0) {

		z = float(tempBoundaries.size());

	}
	else {

		z = float(tempBoundaries.size() + itr->first);

	}

	for (float i = itr->first; i < z; i++) {

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

void Player::SetExits(std::vector<Play::Point2D> levelExits) {

	SetPosition(levelExits[0]);
	endPos = levelExits[1];

}

void Player::SetColour(std::vector<Play::Colour> colourList, int newColour) {

	playerColour[0] = colourList[newColour];

}

void Player::DrawCharacter() {

	Play::DrawRect(characterPos, { characterPos.x + float(DISPLAY_TILE), characterPos.y + float(DISPLAY_TILE) }, playerColour[0], true);
	for (int i = 8; i > 5; i--) {
		if (playerColour[0].blue == 0 && playerColour[0].red == 0 && playerColour[0].green == 0) {
			Play::DrawCircle({ characterPos.x + float(DISPLAY_TILE * 0.5) + playerDirection.x, characterPos.y + float(DISPLAY_TILE * 0.5) + playerDirection.y }, i, Play::cWhite);
		}
		else {
			Play::DrawCircle({ characterPos.x + float(DISPLAY_TILE * 0.5) + playerDirection.x, characterPos.y + float(DISPLAY_TILE * 0.5) + playerDirection.y }, i, Play::cBlack);
		}
	}
	

}

bool Player::HandleControls(std::vector<Play::KeyboardButton> gameControls) {
	//Player moves faster when holding SHIFT
	if (Play::KeyDown(Play::KEY_SHIFT)) {
		posIncrease = 2.0f;
	}
	else {
		posIncrease = 1.0f;
	}

	// Player moves up
	if (Play::KeyPressed(gameControls[1])) {

		newPos = { characterPos.x, characterPos.y + (posIncrease * DISPLAY_TILE) };
		playerDirection = { 0.0f, DISPLAY_TILE * 0.25f };
		CheckCollision();

	}
	// Player moves down
	else if (Play::KeyPressed(gameControls[3])) {

		newPos = { characterPos.x, characterPos.y - (posIncrease * DISPLAY_TILE) };
		playerDirection = { 0.0f, -DISPLAY_TILE * 0.25f };
		CheckCollision();

	}
	// Player moves to the left
	else if (Play::KeyPressed(gameControls[0])) {

		newPos = { characterPos.x - (posIncrease * DISPLAY_TILE), characterPos.y };
		playerDirection = { -DISPLAY_TILE * 0.25f, 0.0f };
		CheckCollision();

	}
	// Player moves to the right
	else if (Play::KeyPressed(gameControls[2])) {

		newPos = { characterPos.x + (posIncrease * DISPLAY_TILE), characterPos.y };
		playerDirection = { DISPLAY_TILE * 0.25f, 0.0f };
		CheckCollision();

	}

	// Check whether the player has reached the end of the current level
	if (endPos == characterPos) {

		return true;

	}
	else {

		return false;

	}

}