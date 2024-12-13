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

	CONTINUE,
	START,
	OPTIONS,
	TROPHIES,
	EXIT

};

enum OptionScreen {

	VOLUME,
	CUTSCENES,
	DIFFICULTY,
	CONTROLS,
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
	void LoadInfo(std::string keyword, int max, std::vector<std::string>& existingVector, std::map<std::string, std::vector<std::string>>& existingMap, std::vector<bool> myVector = { true });

	void DrawOptions(std::vector<std::string> myVector);
	void DrawAdditional(std::vector<std::string> tempStr);
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
	bool levelStart;
	std::vector<bool> defVector;

	bool visibleCutscenes; // true = on
	bool currentControls;
	GameDifficulty currentDifficulty;
	int currentPlayerColour;
	std::vector<Play::Colour> playerColours; // rotate between player sprites
	float gameVolume;
	std::vector<Play::KeyboardButton> gameControls;

	std::vector<std::string> MainMenu;
	std::vector<std::string> OptionMenu;
	std::vector<std::string> TrophyMenu;
	std::vector<bool> TrophyGet;

	std::map<std::string, std::vector<std::string>> TrophyMap;
	std::vector<std::string> currentOptionValues;
	std::vector<std::string> gameDifficultyOptions;
	std::vector<std::string> tipCarousel;
	std::map<std::string, std::vector<std::string>> TipMap;
	
	GameScreen currentGameScreen;
	MainMenuScreen currentMainMenuOption;
	OptionScreen currentOptionMenuOption;
	TrophyScreen currentTrophyMenuOption;

	LevelManager* currentLevel = new LevelManager;
	Player* currentPlayer = new Player;

};

#endif