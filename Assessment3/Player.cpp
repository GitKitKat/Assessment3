/* Includes: 
This file's header */
#include "Player.h"

Player::Player() {
	/* Definitions: */
	// 
	creaturePos = { 0.0f, 0.0f }; 
	newPos = { 0.0f, 0.0f };
	playerDirection = { 0.0f, 0.0f }; 
	posIncrease = 1.0f; // default movement is one tile at a time

}

Player::~Player() {
	// Destructor
}

void Player::SetExits(std::vector<Play::Point2D> levelExits) {

	SetPosition(levelExits[0]);
	endPos = levelExits[1];

}

void Player::SetColour(std::vector<Play::Colour> colourList, int newColour) {
	playerColour[0] = colourList[newColour];
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

		newPos = { creaturePos.x, creaturePos.y + (posIncrease * DISPLAY_TILE) };
		playerDirection = { 0.0f, DISPLAY_TILE * 0.25f };
		CheckCollision();

	}
	// Player moves down
	else if (Play::KeyPressed(gameControls[3])) {

		newPos = { creaturePos.x, creaturePos.y - (posIncrease * DISPLAY_TILE) };
		playerDirection = { 0.0f, -DISPLAY_TILE * 0.25f };
		CheckCollision();

	}
	// Player moves to the left
	else if (Play::KeyPressed(gameControls[0])) {

		newPos = { creaturePos.x - (posIncrease * DISPLAY_TILE), creaturePos.y };
		playerDirection = { -DISPLAY_TILE * 0.25f, 0.0f };
		CheckCollision();

	}
	// Player moves to the right
	else if (Play::KeyPressed(gameControls[2])) {

		newPos = { creaturePos.x + (posIncrease * DISPLAY_TILE), creaturePos.y };
		playerDirection = { DISPLAY_TILE * 0.25f, 0.0f };
		CheckCollision();

	}

	// Check whether the player has reached the end of the current level
	if (endPos == creaturePos) {
		return true;
	}
	else {
		return false;
	}

}

void Player::DrawCharacter() {

	Play::DrawRect(creaturePos, { creaturePos.x + (DISPLAY_TILE), creaturePos.y + (DISPLAY_TILE) }, playerColour[0], true);
	// Draws a circle on the player to note its current direction
	for (int i = 8; i > 5; i--) { // Changes the colour of the circle if the player's colour is dark (i.e. blue or black)
		if (playerColour[0].red == 0 && playerColour[0].green == 0) {
			Play::DrawCircle({ creaturePos.x + (DISPLAY_TILE * 0.5f) + playerDirection.x, creaturePos.y + (DISPLAY_TILE * 0.5f) + playerDirection.y }, i, Play::cWhite);
		}
		else {
			Play::DrawCircle({ creaturePos.x + (DISPLAY_TILE * 0.5f) + playerDirection.x, creaturePos.y + (DISPLAY_TILE * 0.5f) + playerDirection.y }, i, Play::cBlack);
		}
	}

}