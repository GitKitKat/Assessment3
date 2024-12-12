#include "Player.h"

Player::Player() {

	playerPos = { 0.0f, 0.0f };

}

Player::~Player() {



}

void Player::SetPosition(Play::Point2D userPos) {

	playerPos = userPos;

}

void Player::SetColour(Play::Colour arr[], int newColour) {

	playerColour[0] = arr[newColour];

}

void Player::DrawPlayer() {

	//Play::DrawSprite("player1_still", playerPos, 0);
	Play::DrawRect(playerPos, { playerPos.x + float(DISPLAY_TILE), playerPos.y + float(DISPLAY_TILE) }, playerColour[0], true);
	for (int i = 10; i > 0; i--) {

		Play::DrawCircle({ playerPos.x + float(DISPLAY_TILE * 0.5), playerPos.y + float(DISPLAY_TILE * 0.5) }, i, Play::cBlack);

	}
	

}

/*void HandleControls() {

	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	if (Play::KeyDown(Play::KEY_SHIFT)) {

		Play::SetSprite(obj_player, "player1_fast", 0);

	}

	if (Play::KeyDown(Play::KEY_UP)) {

		obj_player.velocity = { 0, 4 };
		Play::SetSprite(obj_player, "player1_walk", 0);

	}
	else if (Play::KeyDown(Play::KEY_DOWN)) {

		obj_player.velocity = { 0, -2 };
		Play::SetSprite(obj_player, "player1_walk", 0);

	}
	else if (Play::KeyDown(Play::KEY_LEFT)) {

		obj_player.velocity = { -2, 0 };
		Play::SetSprite(obj_player, "player1_walk", 0);

	}
	else if (Play::KeyDown(Play::KEY_RIGHT)) {

		obj_player.velocity = { 4, 0 };
		Play::SetSprite(obj_player, "player1_walk", 0);

	}

	else {

		Play::SetSprite(obj_player, "player1_still", 0);

	}

	Play::UpdateGameObject(obj_player);

	if (Play::IsLeavingDisplayArea(obj_player)) {

		obj_player.pos = obj_player.oldPos;

	}

}*/