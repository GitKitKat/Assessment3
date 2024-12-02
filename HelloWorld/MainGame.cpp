#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "Config.h"
#include "GameManager.h"

/* Globals */
//
//GameState* gamePtr = new GameState;
//CreatureObject* creaturePtr = new CreatureObject;

void MainGameEntry(PLAY_IGNORE_COMMAND_LINE);
bool MainGameUpdate(float elapsedTime);

void HandlePlayerControls();
void UpdateFan();
void UpdateTools();
void UpdateCoinsAndStars();

int MainGameExit();

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE ) {

	GameManager* gameManager = GameManager::GetGameManager();
	gameManager->Initialise();

	//Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	//Play::CentreAllSpriteOrigins();

	//Play::LoadBackground("Data\\Backgrounds\\background.png");
	//Play::StartAudioLoop("music");
	Play::CreateGameObject(TYPE_PLAYER, { 115,600 }, 50, "playerTD");

	int id_fan = Play::CreateGameObject(TYPE_FAN, { 1140,503 }, 0, "fan");

	Play::GetGameObject(id_fan).velocity = { 0,-3 };
	Play::GetGameObject(id_fan).animSpeed = 1.0f;

}

//! @brief Called by PlayBuffer every frame (60 times a second!)
//! @param time, the amount of time passed since the player was created.
//! @return a boolean, whether or not the game has been quit (escaped).
bool MainGameUpdate( float elapsedTime ) {
	
	GameManager* gameManager = GameManager::GetGameManager();
	return gameManager->Update(elapsedTime);

//	gamePtr->SetTime(elapsedTime);

	// Colour initialize: black
//	Play::ClearDrawingBuffer( Play::cBlack );

	//Play::DrawFontText("font32px_10x10", gamePtr->CurrentScore(), {30, 20}, LEFT);

	//Play::DrawBackground();
	HandlePlayerControls();
	UpdateFan();
	UpdateTools();
	UpdateCoinsAndStars();

	// Buffer is drawn
	Play::PresentDrawingBuffer();
	
	// Program is closed when ESC key is pressed
	return Play::KeyDown( KEY_ESCAPE );

}

// consider encapsulation (Game object, Player object, Trap object, Enemy object)
	// Grid is drawn

	// Handles player input
	// note player inputs (up, down, left, right)

	// Player is drawn
	// note player X & Y

void HandlePlayerControls() {

	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	if (Play::KeyDown(Play::KEY_UP)) {

		obj_player.velocity = { 0, 4 };
		//Play::SetSprite(obj_agent8, "agent8_climb", 0.25f);

	}
	else if (Play::KeyDown(Play::KEY_DOWN)) {

		obj_player.acceleration = { 0,-1 };
		//Play::SetSprite(obj_agent8, "agent8_fall", 0);

	}
	else if (Play::KeyPressed(Play::KEY_SPACE)) {

		// magiATK

	}
	else {

		//Play::SetSprite(obj_agent8, "agent8_hang", 0.02f);
		obj_player.velocity *= 0.5f;
		obj_player.acceleration = { 0,0 };

	}

	Play::UpdateGameObject(obj_player);

	if (Play::IsLeavingDisplayArea(obj_player)) {

		obj_player.pos = obj_player.oldPos;

	}

	//Play::DrawLine({ obj_player.pos.x,720 }, obj_player.pos, Play::cWhite);
	//Play::DrawObjectRotated(obj_player);

}

void UpdateFan() {

	GameObject& obj_fan = Play::GetGameObjectByType(TYPE_FAN);

	if (Play::RandomRoll(50) == 50) {

		int id = Play::CreateGameObject(TYPE_TOOL, obj_fan.pos, 50, "driver");
		GameObject& obj_tool = Play::GetGameObject(id);
		obj_tool.velocity = Point2f(-8, Play::RandomRollRange(-1, 1) * 6);

		if (Play::RandomRoll(2) == 1) {

			Play::SetSprite(obj_tool, "spanner", 0);
			obj_tool.radius = 100;
			obj_tool.velocity.x = -4;
			obj_tool.rotSpeed = 0.1f;

		}

		Play::PlayAudio("tool");

	}

	if (Play::RandomRoll(150) == 1) {

		int id = Play::CreateGameObject(TYPE_COIN, obj_fan.pos, 40, "coin");
		GameObject& obj_coin = Play::GetGameObject(id);
		obj_coin.velocity = { -3, 0 };
		obj_coin.rotSpeed = 0.1f;

	}

	Play::UpdateGameObject(obj_fan);

	if (Play::IsLeavingDisplayArea(obj_fan)) {

		obj_fan.pos = obj_fan.oldPos;
		obj_fan.velocity.y *= -1;

	}

	Play::DrawObject(obj_fan);

}

void UpdateTools() {

	GameObject& obj_agent8 = Play::GetGameObjectByType(TYPE_AGENT8);
	std::vector<int> vTools = Play::CollectGameObjectIDsByType(TYPE_TOOL);

	for (int id : vTools) {

		GameObject& obj_tool = Play::GetGameObject(id);

		if (Play::IsColliding(obj_tool, obj_agent8)) {

			Play::StopAudio("music");
			Play::PlayAudio("die");
			obj_agent8.pos = { -100,-100 };

		}

		Play::UpdateGameObject(obj_tool);

		if (Play::IsLeavingDisplayArea(obj_tool, Play::VERTICAL)) {

			obj_tool.pos = obj_tool.oldPos;
			obj_tool.velocity.y *= -1;

		}

		Play::DrawObjectRotated(obj_tool);

		if (!Play::IsVisible(obj_tool)) {

			Play::DestroyGameObject(id);

		}

	}

}

void UpdateCoinsAndStars() {

	GameObject& obj_agent8 = Play::GetGameObjectByType(TYPE_AGENT8);
	std::vector<int> vCoins = Play::CollectGameObjectIDsByType(TYPE_COIN);

	for (int id_coin : vCoins) {

		GameObject& obj_coin = Play::GetGameObject(id_coin);
		bool hasCollided = false;

		if (Play::IsColliding(obj_coin, obj_agent8)) {

			for (float rad{ 0.25f }; rad < 2.0f; rad += 0.5f) { // radial collision

				int id = Play::CreateGameObject(TYPE_STAR, obj_agent8.pos, 0, "star");
				GameObject& obj_star = Play::GetGameObject(id);
				obj_star.rotSpeed = 0.1f;
				obj_star.acceleration = { 0.0f, -0.5f };
				Play::SetGameObjectDirection(obj_star, 16, rad * PLAY_PI);

			}

			hasCollided = true;
			//gamePtr->SetScore(500);
			Play::PlayAudio("collect");

		}

		Play::UpdateGameObject(obj_coin);
		Play::DrawObjectRotated(obj_coin);

		if (!Play::IsVisible(obj_coin) || hasCollided) {

			Play::DestroyGameObject(id_coin);

		}

	}

	std::vector<int> vStars = Play::CollectGameObjectIDsByType(TYPE_STAR);

	for (int id_star : vStars) {

		GameObject& obj_star = Play::GetGameObject(id_star);
		Play::UpdateGameObject(obj_star);
		Play::DrawObjectRotated(obj_star);

		if (!Play::IsVisible(obj_star)) {

			Play::DestroyGameObject(id_star);

		}

	}

}

// Gets called once when the player quits the game 
int MainGameExit(void) {

	GameManager* gameManager = GameManager::GetGameManager();
	gameManager->Deinitialise();

	Play::DestroyManager();
	return PLAY_OK;

}