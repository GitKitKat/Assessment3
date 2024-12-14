#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

/* Includes:
Standard header files */
#include <string>
#include <map>
#include <vector>

/* Header files */
#include "Config.h"
#include "LevelManager.h"
#include "Player.h"

//! @brief Each number represents a different screen displayed in the game window.
enum GameScreen {
	//! @brief The opening screen. Non-interactive, displays the game title.
	SPLASH,
	//! @brief Interactive screen after the SPLASH, leads to the OPTION, TROPHY and PLAY screens.
	MAIN_MENU,
	//! @brief Interactive. Leads to the OptionScreen enum. Displays game and player controls.
	OPTION_MENU,
	//! @brief Interactive. Leads to the TrophyScreen enum. Displays game achievements.
	TROPHY_MENU,
	//! @brief Leads to the interactive game.
	PLAY,
	//! @brief Leads to the conclusion screen after a game ends. 
	GAME_END,
	//! @brief Leads to closing the game window.
	GAME_EXIT

};

// Within the 'MAIN_MENU' screen, accessible from the GameScreen enum.
//! @brief Each number represents an interactive element related to the game.
enum MainMenuScreen {
	//! @brief Only appears after a game has been entered once. Leads a user to continue from where they stopped.
	CONTINUE,
	//! @brief Leads to creating a new game with reset positions.
	START,
	//! @brief Leads to the OPTIONS_MENU screen.
	OPTIONS,
	//! @brief Leads to the TROPHY_MENU screen.
	TROPHIES,
	//! @brief Leads to the GAME_EXIT screen.
	EXIT

};

// Within the 'OPTIONS' screen, accessible from the MainMenuScreen enum.
//! @brief Excluding the last (back to previous screen), each number represents the current audio & visual settings of the game (can be changed).
enum OptionScreen {
	//! @brief Leads to changing the current sound output.
	VOLUME,
	//! @brief Leads to changing the display (on/off) of character dialogues.
	CUTSCENES,
	//! @brief Leads to changing the game difficulty.
	DIFFICULTY,
	//! @brief Leads to changing the game controls (input keys).
	CONTROLS,
	//! @brief Leads to chaning the player's visuals.
	PLAYER,
	//! @brief Returns the user to the previous screen (MAIN_MENU).
	OPTION_BACK

};

// Within the 'TROPHIES' screen, accessible from the MainMenuScreen enum.
//! @brief Excluding the last (back to previous screen), each number represents a different collectible achievement in the game.
enum TrophyScreen {
	// The trophy enums are used to register the user's current choice and the displayed achievement (if achieved during a game).
	TROPHY1,
	TROPHY2,
	TROPHY3,
	TROPHY4,
	TROPHY5,
	//! @brief Returns the user to the previous screen (MAIN_MENU).
	TROPHY_BACK

};

class GameManager {

public:
	/* Declarations: */
	// Constructor
	GameManager();
	// Destructor
	~GameManager();

	// Reads and stores data from the ItemList text file in respective variables
	void LoadInfo(std::string keyword, int max, std::vector<std::string>& existingVector, std::map<std::string, std::vector<std::string>>& existingMap, std::vector<bool> myVector = { true });

	/* UI functions: */
	// Handles user interaction (option selection, moving between options) before a game starts
	int MenuInteraction(int optionValue, int maxValue);
	// Handles user interaction on the main screen, as it is the only screen that leads to other screens
	void MenuInteraction(int optionValue, int maxValue, GameScreen newScreen);
	// Draws visual elements based on the current screen
	void DrawOptions(std::vector<std::string> myVector);
	// Draws text elements (loaded from the ItemList file) based on the current screen
	void DrawTexts(std::vector<std::string> tempStr);
	// Determines the currently displayed screen and related actions
	bool ScreenUpdate(float elapsedTime);
	// Displays hints related to the game (if the main menu screen is active)
	void ScreenTips(float elapsedTime);

	// Handles interactions between the current active level and player
	void PlayGame(float elapsedTime);

private:
	/* Variables related to:
	Game window */
	// Stores the elasped time since the current screen became active
	float screenTimer;
	// Check whether or not the game window should be open (if true, close window)
	bool inactiveGame;
	
	/* Game Controls */
	// Determines whether or not character dialogues are displayed (displayed if true)
	bool visibleCutscenes;
	// Determines the available user input (if true, inputs are registered through the W,A,S, and D keys)
	bool currentControls;
	// Stores the available user input (W,A,S, and D or Arrow keys) 
	std::vector<Play::KeyboardButton> gameControls;
	// Stores the game's difficulty as a number
	int currentDifficulty;
	// Stores the colours available to a player
	std::vector<Play::Colour> playerColours;
	// Index used (with playerColours) to determine the displayed player colour
	int currentPlayerColour;
	// Determines the level of sound output
	float gameVolume;
	
	/* Gameplay */
	// Check whether or not a new level was entered (if true, resets character positions)
	bool levelStart;
	// Handles the game level (displayed level, enemies, NPCs, positions of entrance, exit, and traps)
	LevelManager* currentLevel = new LevelManager;
	// Handles the player (player colour, position, controls)
	Player* currentPlayer = new Player;

	/* Displayed game screen */
	GameScreen currentGameScreen;
	MainMenuScreen currentMainMenuOption;
	std::vector<std::string> MainMenu;
	std::map<std::string, std::vector<std::string>> TipMap;
	std::vector<std::string> tipCarousel;
	int carouselIndex;

	OptionScreen currentOptionMenuOption;
	std::vector<std::string> OptionMenu;
	std::vector<std::string> currentOptionValues;
	std::vector<std::string> gameDifficultyOptions;

	TrophyScreen currentTrophyMenuOption;
	std::map<std::string, std::vector<std::string>> TrophyMap;
	std::vector<std::string> TrophyMenu;
	std::vector<bool> TrophyGet;

};

#endif