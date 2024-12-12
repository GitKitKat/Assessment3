#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Config.h"
#include "LevelManager.h"
#include "Player.h"

#include <string>
#include <map>
#include <vector>

enum GameScreen {

	SPLASH,
	MAIN_MENU,
	OPTION_MENU,
	TROPHY_MENU,
	PLAY,
	GAME_END,
	GAME_EXIT

};

enum MainMenuScreen {

	START,
	OPTIONS,
	TROPHIES,
	EXIT

};

enum OptionScreen {

	VOLUME,
	CUTSCENES,
	DIFFICULTY,
	PLAYER,
	OPTION_BACK

};

enum TrophyScreen {

	TROPHY1,
	TROPHY2,
	TROPHY3,
	TROPHY4,
	TROPHY5,
	TROPHY_BACK

};

enum GameDifficulty {

	EASY = 1,
	NORMAL,
	HARD

};

class GameManager {

public:
	GameManager();
	~GameManager();

	void PlayGame(float elapsedTime);

	void DrawOptions(std::vector<std::string> myVector);
	bool ScreenUpdate(float elapsedTime);
	void ScreenTips(float elapsedTime);

	int MenuInteraction(int optionValue, int maxValue);
	void MenuInteraction(int optionValue, int maxValue, GameScreen newScreen);
	//void OptionMenuInteraction();
	//void TrophyMenuInteraction();
	//void TrophyDisplay();

private:
	float screenTimer;
	bool inactiveGame;
	int carouselIndex;
	
	bool visibleCutscenes; // true = on
	GameDifficulty currentDifficulty;
	int currentPlayerColour;
	Play::Colour playerColours[3] = { Play::cCyan, Play::cMagenta, Play::cOrange }; // rotate between player sprites
	float gameVolume;

	std::vector<std::string> MainMenu = { "NEW GAME", "OPTIONS", "TROPHIES", "EXIT" };
	std::vector<std::string> OptionMenu = { "VOLUME", "CUTSCENES", "DIFFICULTY", "PLAYER" };
	std::vector<std::string> TrophyMenu = { "Rookie Puzzler", "Aspiring Adventurer", "Maze Whisperer", "Slow and Steady wins the race", "Speed, I am speed" };
	std::vector<bool> TrophyGet = { false, false, false, false, false };

	//std::map<TrophyScreen, std::vector<std::string>> TrophyMap;
	std::vector<std::string> currentOptionValues = { " ", "ON", "EASY", " ", " " };

	std::vector<std::string> gameDifficultyOptions = { "EASY", "NORMAL", "HARD" };
	std::vector<std::string> carousel = { "TIP 1", "TIP 2", "TIP 3", "TIP 4", "TIP 5" };
	
	GameScreen currentGameScreen;
	MainMenuScreen currentMainMenuOption;
	OptionScreen currentOptionMenuOption;
	TrophyScreen currentTrophyMenuOption;

	LevelManager* currentLevel = new LevelManager;
	Player* currentPlayer = new Player;

};

#endif