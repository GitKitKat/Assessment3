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
#include "NPC.h"

// Each number represents a different screen displayed in the game window.
enum GameScreen {
	// The opening screen. Non-interactive, displays the game title.
	SPLASH,
	// Interactive screen after the SPLASH, leads to the OPTION, TROPHY and PLAY screens.
	MAIN_MENU,
	// Interactive. Leads to the OptionScreen enum. Displays game and player controls.
	OPTION_MENU,
	// Interactive. Leads to the TrophyScreen enum. Displays game achievements.
	TROPHY_MENU,
	// Leads to the interactive game.
	PLAY,
	// Leads to the conclusion screen after a game ends. 
	GAME_END,
	// Leads to closing the game window.
	GAME_EXIT

};

// Within the 'MAIN_MENU' screen, accessible from the GameScreen enum.
// Each number represents an interactive element related to the game.
enum MainMenuScreen {
	// Only appears after a game has been entered once. Leads a user to continue from where they stopped.
	CONTINUE,
	// Leads to creating a new game with reset positions.
	START,
	// Leads to the OPTIONS_MENU screen.
	OPTIONS,
	// Leads to the TROPHY_MENU screen.
	TROPHIES,
	// Leads to the GAME_EXIT screen.
	EXIT

};

// Within the 'OPTIONS' screen, accessible from the MainMenuScreen enum.
// Excluding the last (back to previous screen), each number represents the current audio & visual settings of the game (can be changed).
enum OptionScreen {
	// Leads to changing the current sound output.
	VOLUME,
	// Leads to changing the display (on/off) of character dialogues.
	CUTSCENES,
	// Leads to changing the game difficulty.
	DIFFICULTY,
	// Leads to changing the game controls (input keys).
	CONTROLS,
	// Leads to chaning the player's visuals.
	PLAYER,
	// Returns the user to the previous screen (MAIN_MENU).
	OPTION_BACK

};

class GameManager {

public:
	/* Declarations: */
	// Constructor
	GameManager();
	// Destructor
	~GameManager();

	// Reads and stores data from the ItemList text file in respective variables
	void LoadInfo(std::string keyword, std::string file, int max, std::vector<std::string>& existingVector, std::map<std::string, std::vector<std::string>>& existingMap, std::vector<bool> myVector = { true });
	void PrintInfo(std::string keyword, std::string file, std::vector<std::string> itemName, std::map<std::string, std::vector<std::string>> itemMap, std::vector<bool> itemGotten);

	/* UI functions: */
	// Handles user interaction (option selection, moving between options) before a game starts
	int MenuInteraction(int optionValue, int maxValue);
	// Handles user interaction on the main screen, as it is the only screen that leads to other screens
	int MenuInteraction(int optionValue, int maxValue, GameScreen newScreen);
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
	void DrawInteraction(std::vector<std::string> interactionDesc, std::vector<std::string> interactionChoices);
	void InteractionManager(std::vector<std::string> interactionDesc, std::vector<std::string> interactionChoices);
	void DrawDialogue();

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
	int characterScene[1];
	Play::Point2D playerOldPos;
	std::vector<bool> updateInteraction;
	int holdInteraction[3];
	// Handles the game level (displayed level, enemies, NPCs, positions of entrance, exit, and traps)
	LevelManager* currentLevel = new LevelManager;
	// Handles the player (player colour, position, controls)
	Player* currentPlayer = new Player;
	NPC* currentNPCs = new NPC;
	//bool NPCInteraction;

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

	int currentTrophyMenuOption;
	std::map<std::string, std::vector<std::string>> TrophyMap;
	std::vector<std::string> TrophyMenu;
	std::vector<bool> TrophyGet;
	std::string itemsFile;
	std::string trophyFile;

	std::vector<std::string> EndMenu;
	int EndGame;

};

#endif