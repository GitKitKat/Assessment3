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

	TrophyMenu.push_back("");
	TrophyMap[""] = TrophyMenu;
	LoadInfo(0, TROPHY_BACK);

	visibleCutscenes = true;
	currentDifficulty = EASY;
	currentPlayerColour = 1;

	currentGameScreen = SPLASH;
	currentMainMenuOption = START;
	currentOptionMenuOption = VOLUME;
	currentTrophyMenuOption = TROPHY1;

}

GameManager::~GameManager() {



}

void GameManager::PlayGame(float elapsedTime) {

	std::vector<Play::Point2D> userPos = currentLevel->GetLevel();

	if (levelStart) {

		currentPlayer->SetPosition(userPos);
		levelStart = false;

	}

	currentPlayer->SetBoundaries(currentDifficulty, currentLevel->GetBoundaries());
	currentPlayer->HandleControls();
	currentPlayer->DrawPlayer();

	screenTimer += elapsedTime;
	Play::PresentDrawingBuffer();

}

void GameManager::LoadInfo(int min, int max) {

	std::ifstream inFile("Data\\Textfiles\\ItemList.txt");
	std::string keyword = "[Trophy]";
	std::string newLine;
	std::string holdItemName;
	std::vector<std::string> holdItemDesc = { "" };
	bool itemFound = false;

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

				TrophyMap.erase("");

				if (newLine.rfind("(Name)", 0) == 0) {

					holdItemName = newLine.substr(6);

					if (TrophyMenu[0] == "") {

						TrophyMenu[0] = holdItemName;

					}
					else {

						TrophyMenu.push_back(holdItemName);

					}

				}

				else if (newLine.rfind("(Desc)", 0) == 0) {

					if (holdItemDesc[0] == "") {

						holdItemDesc[0] = newLine.substr(6);

					}
					else {

						holdItemDesc.push_back(newLine.substr(6));

					}

				}

				else if (newLine.rfind("(Get)", 0) == 0) {

					if (newLine.substr(5) == "F") {

						TrophyGet.push_back(false);

					}
					else {

						TrophyGet.push_back(true);

					}

				}

				else if (newLine == "[End]") {

					TrophyMap.insert({ holdItemName, holdItemDesc });
					holdItemDesc.clear();
					holdItemDesc = { "" };
					min++;

					if (min == max) {

						itemFound = false;

						break;

					}

				}

			}

		}

	}

}

void GameManager::DrawOptions(std::vector<std::string> myVector) {

	float j = DISPLAY_TILE * (4 / 2);

	for (int i = 0; i < myVector.size(); i++) {

		int b = 1;

		if (currentGameScreen == TROPHY_MENU) {

			if (TrophyGet[i] == false) {

				myVector[i] = "???";

			}
			if (currentTrophyMenuOption != TROPHY_BACK) {

				if (TrophyGet[currentTrophyMenuOption] == true) {

					std::vector<std::string> tempStr = TrophyMap.at(myVector[currentTrophyMenuOption]);
					for (int a = 0; a < tempStr.size() - 1; a++) {

						Play::DrawFontText("32px", tempStr[a], {DISPLAY_TILE * 2, (DISPLAY_HEIGHT / 2 + DISPLAY_TILE) - (DISPLAY_TILE * b)}, Play::LEFT);
						b++;

					}

					int a = tempStr.size() - 1;
					const char* charPtr = tempStr[a].c_str();
					Play::DrawDebugText({ DISPLAY_TILE * 2, (DISPLAY_HEIGHT / 2 + DISPLAY_TILE) - (DISPLAY_TILE * b) }, charPtr, Play::cWhite, false);

				}
			}

		}

		if (currentGameScreen == OPTION_MENU) {

			Play::DrawFontText("32px", currentOptionValues[i], { (DISPLAY_WIDTH / 2) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT / 2) + j }, Play::LEFT);

		}

		Play::DrawFontText("32px", myVector[i], { DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) + j }, Play::LEFT);

		j -= DISPLAY_TILE;

	}

	if (currentGameScreen == OPTION_MENU) {

		Play::Point2D tempPos = { (DISPLAY_WIDTH / 2) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT / 2) + DISPLAY_TILE * (- PLAYER + 1.5f) };
		Play::DrawRect(tempPos, { tempPos.x + DISPLAY_TILE, tempPos.y + DISPLAY_TILE }, Play::cWhite, false);
		currentPlayer->SetPosition(tempPos);
		currentPlayer->DrawPlayer();

	}

	if (currentGameScreen == TROPHY_MENU || currentGameScreen == OPTION_MENU) {

		Play::DrawFontText("32px", "BACK", { DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) + j }, Play::LEFT);

	}

}

void GameManager::ScreenTips(float elapsedTime) {

	if (screenTimer < 6.0f) {

		Play::DrawFontText("72px", carousel[carouselIndex], { DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 2 }, Play::LEFT);
		screenTimer += elapsedTime;

	} else {

		if (carouselIndex == 4) {

			carouselIndex = 0;

		}
		else {

			carouselIndex++;

		}

		screenTimer = 0.0f;

	}

}

int GameManager::MenuInteraction(int optionValue, int maxValue) {

	float j = 50 * (4 / 2);
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH / 2) - (DISPLAY_TILE * 0.5f), (DISPLAY_HEIGHT / 2) + j - (optionValue * 50) }, Play::CENTRE);

	if (Play::KeyPressed(Play::KEY_W) && optionValue != 0) {

		optionValue--;

	}
	else if (Play::KeyPressed(Play::KEY_S) && optionValue != maxValue) {

		optionValue++;

	}

	return optionValue;

}

void GameManager::MenuInteraction(int optionValue, int maxValue, GameScreen newScreen) {

	float j = 50 * (4 / 2);
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH / 2) - (DISPLAY_TILE * 0.5f), (DISPLAY_HEIGHT / 2) + j - (optionValue * 50) }, Play::CENTRE);

	if (Play::KeyPressed(Play::KEY_W) && currentMainMenuOption != 0) {

		optionValue--;
		currentMainMenuOption = MainMenuScreen(optionValue);

	}
	else if (Play::KeyPressed(Play::KEY_S) && currentMainMenuOption != maxValue) {

		optionValue++;
		currentMainMenuOption = MainMenuScreen(optionValue);

	}
	else if (Play::KeyPressed(Play::KEY_SPACE)) {

		if (newScreen == GAME_EXIT) {

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

			Play::ClearDrawingBuffer(Play::cOrange);
			Play::DrawFontText("72px", "THE REQUEST", { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, Play::CENTRE);

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
		case START:
			
			MenuInteraction(int(START), int(EXIT), PLAY);

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

		Play::DrawFontText("72px", "OPTIONS", { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 + 200 }, Play::CENTRE);
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
					currentOptionValues[1] = "ON";
				}
				else {
					currentOptionValues[1] = "OFF";
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
				currentOptionValues[2] = gameDifficultyOptions[i - 1];

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

		Play::DrawFontText("72px", "TROPHIES", { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 + 200 }, Play::CENTRE);
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

			currentGameScreen = MAIN_MENU;
			currentMainMenuOption = START;
			screenTimer = 0.0f;

			delete currentLevel;
			delete currentPlayer;
			currentLevel = nullptr;
			currentPlayer = nullptr;
			inactiveGame = true;

		}

		Play::PresentDrawingBuffer();
		break;
	default:
		// Invalid state, exit game.
		inactiveGame = true;

	}

	return inactiveGame;

}