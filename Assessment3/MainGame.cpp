#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER

/* Includes:
Header files */
#include "Play.h"
#include "Config.h"
#include "GameManager.h"

/* Declarations */
//
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE);
bool MainGameUpdate(float elapsedTime);
int MainGameExit();

// A gloabl pointer to the GameManager, where the content within a game window is decided
GameManager* newGame = new GameManager();

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE) {
	// Creates a display window
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	Play::CentreAllSpriteOrigins();

}

// Called by PlayBuffer every frame (60 times a second)
bool MainGameUpdate(float elapsedTime) {
	// ScreenUpdate returns a false boolean value if the game has not been quit
	return newGame->ScreenUpdate(elapsedTime);

}

// Called when a player exits the game
int MainGameExit(void) {
	
	delete newGame;
	Play::DestroyManager();
	return PLAY_OK;

}