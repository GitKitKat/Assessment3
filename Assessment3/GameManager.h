#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

/* Includes:
Standard header files */
#include <map>

/* Header files */
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

// Within the MAIN_MENU screen, accessible from the GameScreen enum.
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

// Within the OPTIONS screen, accessible from the MainMenuScreen enum.
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

	// Reads and stores data from the passed text file in respective variables
	void LoadInfo(std::string keyword, std::string file, int max, std::vector<std::string>& existingVector, std::map<std::string, std::vector<std::string>>& existingMap, std::vector<bool> myVector = { true });
	// Prints stored data into a text file, overwriting the previous save
	void PrintInfo(std::string keyword, std::string file, std::vector<std::string> itemName, std::map<std::string, std::vector<std::string>> itemMap, std::vector<bool> itemGotten);

	/* Regarding pre-game UI */
	// Handles user encounter (option selection, moving between options) before a game starts
	int MenuEncounter(int optionValue, int maxValue);
	// Handles user encounter on a screen that leads to other screens
	int MenuEncounter(int optionValue, int maxValue, GameScreen newScreen);
	// Draws visual elements based on the current screen
	void DrawOptions(std::vector<std::string> myVector);
	// Draws text elements (loaded from the ItemList file) based on the current screen
	void DrawTexts(std::vector<std::string> tempStr);
	// Determines the currently displayed screen and related actions
	bool ScreenUpdate(float elapsedTime);
	// Displays hints related to the game (if the main menu screen is active)
	void ScreenTips(float elapsedTime);

	/* Regarding the game/gameplay */
	// Handles encounters between the current active level and player
	void PlayGame(float elapsedTime);
	// Prints NPC dialogue (name, 1-3 lines of dialogue, possible user choices) in white text on a black screen
	void DrawDialogue();
	// Prints text related to an enemy encounter (introduction, player and enemy choices)
	void DrawEncounter(std::vector<std::string> encounterDesc, std::vector<std::string> encounterChoices);
	// Handles player inputs, processes and displays results in an enemy encounter
	void EncounterManager(std::vector<std::string> encounterDesc, std::vector<std::string> encounterChoices);

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
	// Check whether a level will be drawn for the first time (if true, resets character positions)
	bool levelStart;
	// Stores the index of an ancountered enemy
	int characterScene[1];
	// Stores the player's old position if the player is moved (not by the user) (e.g. when displaying the Options menu)
	Play::Point2D playerOldPos;
	// Used to check various changes during an enemy encounter
	std::vector<bool> updateEncounter;
	// Stores the choices made by both enemy and player during an encounter
	std::vector<int> holdEncounterChoices;
	// Handles the game level (displayed level, enemies, NPCs, positions of entrance, exit, and traps)
	LevelManager* currentLevel = new LevelManager;
	// Handles the player (player colour, position, controls)
	Player* currentPlayer = new Player;
	// Handles the NPC (NPC position, dialogue)
	NPC* currentNPCs = new NPC;

	/* Displayed game screen */
	// Stores the value corresponding to the currently displayed game screen (e.g. MAIN_MENU, TROPHY_MENU, PLAY, GAME_END) 
	GameScreen currentGameScreen;
	// Stores the value corresponding to the currently selected MAIN_MENU option
	MainMenuScreen currentMainMenuOption;
	// Stores the value corresponding to the currently selected OPTION_MENU option
	OptionScreen currentOptionMenuOption;
	// Stores the value corresponding to the currently selected TROPHY_MENU option
	int currentTrophyMenuOption;
	// Stores the value corresponding to the currently selected GAME_END menu option
	int currentEndGameOption;
	// Stores the value corresponding to the currently displayed Tip
	int carouselIndex;
	// Stores the displayed titles corresponding to each option in the MAIN_MENU screen
	std::vector<std::string> MainMenu;
	// Stores the displayed information corresponding to each tip shown on the MAIN_MENU screen (tagline, description)
	std::map<std::string, std::vector<std::string>> TipMap;
	// Stores the displayed titles corresponding to each tip shown on the MAIN_MENU screen
	std::vector<std::string> tipCarousel;
	// Stores the displayed titles corresponding to each option in the OPTION_MENU screen
	std::vector<std::string> OptionMenu;
	// Stores the displayed states corresponding to each option in the OPTION_MENU screen (e.g. current volume percentage, current player colour)
	std::vector<std::string> currentOptionValues;
	// Stores the displayed state corresponding to each difficulty setting in the OPTION_MENU screen (e.g. 1 is displayed as EASY)
	std::vector<std::string> gameDifficultyOptions;
	// Stores the displayed information corresponding to each trophy in the TROPHY_MENU screen (tagline, description)
	std::map<std::string, std::vector<std::string>> TrophyMap;
	// Stores the displayed titles corresponding to each trophy in the TROPHY_MENU screen
	std::vector<std::string> TrophyMenu;
	// Stores the status of each trophy (i.e. whether or not the trophy has been achieved)
	std::vector<bool> TrophyGet;
	// Stores the displayed titles corresponding to each option in the GAME_END screen
	std::vector<std::string> EndMenu;
	// Stores the file path for the ItemsList file (data stored: Enemy encounters, game tips' names and descriptions)
	std::string itemsFile;
	// Stores the file path for the TrophyList file (data stored: trophy names and descriptions)
	std::string trophyFile;

};

#endif