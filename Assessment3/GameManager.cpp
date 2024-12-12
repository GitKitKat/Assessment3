/* This file's header */
#include "GameManager.h"

#include "Play.h"
#include "LevelManager.h"
#include <string>
#include <vector>

GameManager::GameManager() {

	screenTimer = 0;
	carouselIndex = 0;
	inactiveGame = false;
	gameVolume = 1.0f;

	visibleCutscenes = true;
	currentDifficulty = EASY;
	currentPlayerColour = 1;

	currentGameScreen = SPLASH;
	currentMainMenuOption = START;
	currentOptionMenuOption = VOLUME;
	currentTrophyMenuOption = TROPHY1;

	//TrophyMap.insert({ TROPHY1, });

}

GameManager::~GameManager() {



}

void GameManager::PlayGame(float elapsedTime) {

	Play::Point2D userPos = currentLevel->GetLevel();
	screenTimer += elapsedTime;

	currentPlayer->SetPosition(userPos);
	currentPlayer->DrawPlayer();

	Play::PresentDrawingBuffer();

}

void GameManager::DrawOptions(std::vector<std::string> myVector) {

	float j = DISPLAY_TILE * (4 / 2);
	//Play::DrawRect({ (DISPLAY_WIDTH / 2) , (DISPLAY_HEIGHT / 2) + j }, { (DISPLAY_WIDTH / 2) + DISPLAY_TILE, (DISPLAY_HEIGHT / 2) + j + DISPLAY_TILE }, Play::cGrey, false);

	for (int i = 0; i < myVector.size(); i++) {

		if (currentGameScreen == TROPHY_MENU) {

			if (TrophyGet[i] == false) {

				myVector[i] = "???";

			}

			//Play::DrawRect({ (DISPLAY_WIDTH / 2) + DISPLAY_TILE, (DISPLAY_HEIGHT / 2) + j - 4 }, { (DISPLAY_WIDTH / 2) + (DISPLAY_TILE * 2), (DISPLAY_HEIGHT / 2) + j + DISPLAY_TILE - 4 }, Play::cWhite, false);

		}

		if (currentGameScreen == OPTION_MENU) {

			Play::DrawFontText("32px", currentOptionValues[i], { (DISPLAY_WIDTH / 2) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT / 2) + j }, Play::LEFT);

		}

		Play::DrawFontText("32px", myVector[i], { DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) + j }, Play::LEFT);

		j -= DISPLAY_TILE;

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
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH / 2) - 50.0f, (DISPLAY_HEIGHT / 2) + j - (optionValue * 50) }, Play::CENTRE);

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
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH / 2) - 50.0f, (DISPLAY_HEIGHT / 2) + j - (optionValue * 50) }, Play::CENTRE);

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
		Play::DrawRect({ (DISPLAY_WIDTH / 2) + (DISPLAY_TILE * 2), (DISPLAY_HEIGHT / 2) - (DISPLAY_TILE * PLAYER) }, { (DISPLAY_WIDTH / 2) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT / 2) - (DISPLAY_TILE * PLAYER) + DISPLAY_TILE }, Play::cWhite, false);
		currentPlayer->SetPosition({ (DISPLAY_WIDTH / 2) + (DISPLAY_TILE * 2), (DISPLAY_HEIGHT / 2) - (DISPLAY_TILE * PLAYER) });
		currentPlayer->DrawPlayer();

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

				if (currentPlayerColour < 3) {

					currentPlayerColour++;

				}
				else {

					currentPlayerColour = 1;

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

		switch (currentTrophyMenuOption) {

		case TROPHY1:


			break;
		case TROPHY_BACK:

			if (Play::KeyPressed(Play::KEY_SPACE)) {

				currentGameScreen = MAIN_MENU;

			}

			break;

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