#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER

/* Header files */
#include "Play.h"
#include "Config.h"
#include "GameManager.h"

/* Function declarations */
//
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE);
bool MainGameUpdate(float elapsedTime);
int MainGameExit();

GameManager* newGame = new GameManager();

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE) {

	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	Play::CentreAllSpriteOrigins();

}

//! @brief Called by PlayBuffer every frame (60 times a second!)
//! @param time, the amount of time passed since the player was created.
//! @return a boolean, whether or not the game has been quit (escaped).
bool MainGameUpdate(float elapsedTime) {

	bool inactiveCheck = newGame->ScreenUpdate(elapsedTime);
	if (inactiveCheck == true) {

		return true;

	}

// Program is closed when ESC key is pressed
	return Play::KeyDown(KEY_ESCAPE);

}

// consider encapsulation (Game object, Player object, Trap object, Enemy object)
	// Grid is drawn

	// Handles player input
	// note player inputs (up, down, left, right)	

	// Player is drawn
	// note player X & Y


// Gets called once when the player quits the game 
int MainGameExit(void) {

	delete newGame;
	Play::DestroyManager();
	return PLAY_OK;

}