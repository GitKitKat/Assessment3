/* This file's header */
#include "GameManager.h"

#include "Play.h"
#include "LevelManager.h"
#include <string>
#include <vector>
#include <fstream>

GameManager::GameManager() {

	screenTimer = 0;
	carouselIndex = 0;
	inactiveGame = false;
	gameVolume = 1.0f;
	levelStart = true;
	currentControls = true;

	visibleCutscenes = true;
	currentDifficulty = EASY;
	currentPlayerColour = 1;

	currentGameScreen = SPLASH;
	currentMainMenuOption = START;
	currentOptionMenuOption = VOLUME;
	currentTrophyMenuOption = TROPHY1;

	playerColours.push_back(Play::cCyan);
	playerColours.push_back(Play::cMagenta);
	playerColours.push_back(Play::cOrange);

	gameControls = { Play::KEY_A, Play::KEY_W, Play::KEY_D, Play::KEY_S };
	MainMenu = { " ", "NEW GAME", "OPTIONS", "TROPHIES", "EXIT" };
	OptionMenu = { "VOLUME", "CUTSCENES", "DIFFICULTY", "CONTROLS", "PLAYER" };

	currentOptionValues = { " ", "ON", "EASY", "WASD", " " };
	gameDifficultyOptions = { "EASY", "NORMAL", "HARD" };

	LoadInfo("[Tip]", 4, tipCarousel, TipMap, defVector);
	LoadInfo("[Trophy]", TROPHY_BACK, TrophyMenu, TrophyMap, TrophyGet);

}

GameManager::~GameManager() {



}

void GameManager::PlayGame(float elapsedTime) {

	std::vector<Play::Point2D> userPos = currentLevel->GetLevel();
	bool levelEnd = false;

	if (levelStart) {

		currentPlayer->SetPosition(userPos);
		levelStart = false;

	}

	currentPlayer->SetBoundaries(currentLevel->GetBoundaries());
	levelEnd = currentPlayer->HandleControls(gameControls);
	currentPlayer->DrawPlayer();

	screenTimer += elapsedTime;

	if (levelEnd) {

		currentLevel->SetLevel();
		levelStart = true;
		levelEnd = false;

	}

	Play::PresentDrawingBuffer();

}

void GameManager::LoadInfo(std::string keyword, int max, std::vector<std::string>& existingVector, std::map<std::string, std::vector<std::string>>& existingMap, std::vector<bool> myVector) {

	std::ifstream inFile("Data\\Textfiles\\ItemList.txt");
	std::string newLine;
	std::string holdItemName;
	std::vector<std::string> holdItemDesc;
	bool itemFound = false;
	int min = 0;

	if (!inFile.is_open()) {

		return;

	}
	else {

		// This loop reads and relays a specified level.
		while (std::getline(inFile, newLine)) {

			if (newLine.rfind(keyword, 0) == 0) {

				itemFound = true;

			}

			if (itemFound == true) {

				if (newLine.rfind("(Name)", 0) == 0) {

					holdItemName = newLine.substr(6);
					existingVector.push_back(holdItemName);

				}

				else if (newLine.rfind("(Desc)", 0) == 0) {

					holdItemDesc.push_back(newLine.substr(6));

				}

				else if (newLine.rfind("(Get)", 0) == 0) {

					if (newLine.substr(5) == "F") {

						myVector.push_back(false);

					}
					else {

						myVector.push_back(true);

					}

				}

				else if (newLine == "[End]") {

					existingMap.insert({ holdItemName, holdItemDesc });
					holdItemDesc.clear();
					min++;

					if (min == max) {

						itemFound = false;

						if (keyword == "[Trophy]") {

							TrophyGet = myVector;

						}

						break;

					}

				}

			}

		}

	}

}

void GameManager::DrawAdditional(std::vector<std::string> tempStr) {

	int b = 1;
	float z;
	if (currentGameScreen == MAIN_MENU) {

		z = (DISPLAY_HEIGHT * 0.5f + DISPLAY_TILE) - ( 3 * DISPLAY_TILE * b);

	}
	else {

		z = (DISPLAY_HEIGHT * 0.5f + DISPLAY_TILE) - (DISPLAY_TILE * b);

	}

	for (int a = 0; a < tempStr.size() - 1; a++) {

		Play::DrawFontText("32px", tempStr[a], { DISPLAY_TILE * 2, z }, Play::LEFT);
		b++;

	}

	int a = tempStr.size() - 1;
	const char* charPtr = tempStr[a].c_str();
	Play::DrawDebugText({ DISPLAY_TILE * 2, z - DISPLAY_TILE }, charPtr, Play::cWhite, false);

}

void GameManager::DrawOptions(std::vector<std::string> myVector) {

	float j = DISPLAY_TILE * (4 * 0.5f);

	for (int i = 0; i < myVector.size(); i++) {

		if (currentGameScreen == TROPHY_MENU) {

			if (TrophyGet[i] == false) {

				myVector[i] = "???";

			}
			if (currentTrophyMenuOption != TROPHY_BACK) {

				if (TrophyGet[currentTrophyMenuOption] == true) {

					DrawAdditional(TrophyMap.at(myVector[currentTrophyMenuOption]));

				}
			}

		}

		if (currentGameScreen == OPTION_MENU) {

			Play::DrawFontText("32px", currentOptionValues[i], { (DISPLAY_WIDTH * 0.5f) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT * 0.5f) + j }, Play::LEFT);

		}

		Play::DrawFontText("32px", myVector[i], { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) + j }, Play::LEFT);

		j -= DISPLAY_TILE;

	}

	if (currentGameScreen == OPTION_MENU) {

		Play::Point2D tempPos = { (DISPLAY_WIDTH * 0.5f) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT * 0.5f) + DISPLAY_TILE * (- PLAYER + 1.5f) };
		Play::DrawRect(tempPos, { tempPos.x + DISPLAY_TILE, tempPos.y + DISPLAY_TILE }, Play::cWhite, false);
		currentPlayer->SetPosition(tempPos);
		currentPlayer->DrawPlayer();

	}

	if (currentGameScreen == TROPHY_MENU || currentGameScreen == OPTION_MENU) {

		Play::DrawFontText("32px", "BACK", { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) + j }, Play::LEFT);

	}

}

void GameManager::ScreenTips(float elapsedTime) {

	if (screenTimer < 6.0f) {

		DrawAdditional(TipMap.at(tipCarousel[carouselIndex]));
		screenTimer += elapsedTime;

	} else {

		if (carouselIndex < TipMap.size() - 1) {

			carouselIndex++;

		}
		else {

			carouselIndex = 0;

		}

		screenTimer = 0.0f;

	}

}

int GameManager::MenuInteraction(int optionValue, int maxValue) {

	float j = 50 * (4 * 0.5f);
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH * 0.5f) - (DISPLAY_TILE * 0.5f), (DISPLAY_HEIGHT * 0.5f) + j - (optionValue * 50) }, Play::CENTRE);

	if (Play::KeyPressed(gameControls[1]) && optionValue != 0) {

		optionValue--;

	}
	else if (Play::KeyPressed(gameControls[3]) && optionValue != maxValue) {

		optionValue++;

	}

	return optionValue;

}

void GameManager::MenuInteraction(int optionValue, int maxValue, GameScreen newScreen) {

	float j = 50 * (4 * 0.5f);
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH * 0.5f) - (DISPLAY_TILE * 0.5f), (DISPLAY_HEIGHT * 0.5f) + j - (optionValue * 50) }, Play::CENTRE);

	if (Play::KeyPressed(gameControls[1]) && currentMainMenuOption != 0) {

		optionValue--;
		currentMainMenuOption = MainMenuScreen(optionValue);

	}
	else if (Play::KeyPressed(gameControls[3]) && currentMainMenuOption != maxValue) {

		optionValue++;
		currentMainMenuOption = MainMenuScreen(optionValue);

	}
	else if (Play::KeyPressed(Play::KEY_SPACE)) {

		if (newScreen == GAME_EXIT) {

			delete currentLevel;
			delete currentPlayer;
			currentLevel = nullptr;
			currentPlayer = nullptr;
			inactiveGame = true;

		}
		else {

			currentGameScreen = newScreen;

		}
	
	}

}

bool GameManager::ScreenUpdate(float elapsedTime) {

	switch (currentGameScreen) {
	case SPLASH:

		if (screenTimer < SPLASH_TIMELIMIT) {

			Play::ClearDrawingBuffer(Play::cBlack);
			Play::DrawCircle({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f }, DISPLAY_TILE * screenTimer, Play::cOrange);
			Play::DrawFontText("72px", "THE REQUEST", { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f }, Play::CENTRE);

			Play::PresentDrawingBuffer();

			screenTimer += elapsedTime;
		}
		else {

			currentGameScreen = MAIN_MENU;
			screenTimer = 0.0f;

		}

		break;
	case MAIN_MENU:

		Play::ClearDrawingBuffer(Play::cBlack);
		ScreenTips(elapsedTime);
		DrawOptions(MainMenu);
		
		switch (currentMainMenuOption) {
		case CONTINUE:

			MenuInteraction(int(CONTINUE), int(EXIT), PLAY);
			levelStart = false;

			break;
		case START:
			
			MenuInteraction(int(START), int(EXIT), PLAY);
			levelStart = true;
			screenTimer = 0.0f;

			break;
		case OPTIONS:
			MenuInteraction(int(OPTIONS), int(EXIT), OPTION_MENU);
			
			break;
		case TROPHIES:
			MenuInteraction(int(TROPHIES), int(EXIT), TROPHY_MENU);

			break;
		case EXIT:
			MenuInteraction(int(EXIT), int(EXIT), GAME_EXIT);

			break;
		}

		Play::PresentDrawingBuffer();
		break;
	case OPTION_MENU:
		Play::ClearDrawingBuffer(Play::cBlack);

		Play::DrawFontText("72px", "OPTIONS", { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f + 200 }, Play::CENTRE);
		DrawOptions(OptionMenu);

		currentOptionMenuOption = OptionScreen(MenuInteraction(int(currentOptionMenuOption), int(OPTION_BACK)));

		switch (currentOptionMenuOption) {

		case VOLUME:

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				if (gameVolume > 0.25f) {

					gameVolume -= 0.25f;

				}
				else {

					gameVolume = 1.0f;

				}

			}

			break;
		case CUTSCENES:

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				visibleCutscenes = !visibleCutscenes;

				if (visibleCutscenes) {
					currentOptionValues[CUTSCENES] = "ON";
				}
				else {
					currentOptionValues[CUTSCENES] = "OFF";
				}
			}

			break;
		case DIFFICULTY:

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				int i = currentDifficulty;

				if (i < int(HARD)) {

					i++;
					currentDifficulty = GameDifficulty(i);

				}
				else {

					i = 1;
					currentDifficulty = GameDifficulty(i);

				}

				currentLevel->SetMode(i);
				currentOptionValues[DIFFICULTY] = gameDifficultyOptions[i - 1];

			}

			break;
		case CONTROLS:

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				currentControls = !currentControls;

				if (currentControls) {

					currentOptionValues[CONTROLS] = "WASD";
					gameControls = { Play::KEY_A, Play::KEY_W, Play::KEY_D, Play::KEY_S };

				}
				else {

					currentOptionValues[CONTROLS] = "Arrow keys";
					gameControls = { Play::KEY_LEFT, Play::KEY_UP, Play::KEY_RIGHT, Play::KEY_DOWN };

				}

			}

			break;
		case PLAYER:

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				if (currentPlayerColour < 2) {

					currentPlayerColour++;

				}
				else {

					currentPlayerColour = 0;

				}

				currentPlayer->SetColour(playerColours, currentPlayerColour);

			}

			break;
		case OPTION_BACK:

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				currentGameScreen = MAIN_MENU;

			}

			break;

		}
		Play::PresentDrawingBuffer();
		break;
	case TROPHY_MENU:
		Play::ClearDrawingBuffer(Play::cBlack);

		Play::DrawFontText("72px", "TROPHIES", { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f + 200 }, Play::CENTRE);
		DrawOptions(TrophyMenu);

		currentTrophyMenuOption = TrophyScreen(MenuInteraction(int(currentTrophyMenuOption), int(TROPHY_BACK)));

		if (currentTrophyMenuOption == TROPHY_BACK) {

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				currentGameScreen = MAIN_MENU;

			}

		}

		Play::PresentDrawingBuffer();
		break;
	case PLAY:

		PlayGame(elapsedTime);

		if (Play::KeyPressed(Play::KEY_ESCAPE)) {
			
			Play::ClearDrawingBuffer(Play::cBlack);

			MainMenu[0] = "CONTINUE";
			currentGameScreen = MAIN_MENU;
			//currentMainMenuOption = START;
			//inactiveGame = true;

		}

		Play::PresentDrawingBuffer();
		break;
	default:
		// Invalid state, exit game.
		inactiveGame = true;

	}

	return inactiveGame;

}