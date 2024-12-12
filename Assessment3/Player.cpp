#include "Player.h"

Player::Player() {

	//playerFile = "player1";
	playerXpos = 200;
	playerYpos = 200;

}

Player::~Player() {



}

void Player::SetPosition(int xPos, int yPos) {

	playerXpos = xPos;
	playerYpos = yPos;

}

/*void Player::HandleControls() {

//	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

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