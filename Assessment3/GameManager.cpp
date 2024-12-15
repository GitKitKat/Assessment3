/* Includes:
This file's header */
#include "GameManager.h"

/* Standard header files */
#include <fstream>
#include <cstdlib>

/* Header files */
#include "Play.h"
#include "LevelManager.h"

GameManager::GameManager() {
	/* Definitions */
	//
	screenTimer = 0;
	carouselIndex = 0;
	inactiveGame = false;
	gameVolume = 1.0f;
	levelStart = true;
	currentControls = true;
	updateInteraction = { false, true, false, false };
	//NPCInteraction = false;
	holdInteraction[0] = 0;
	holdInteraction[1] = 0;
	holdInteraction[2] = -1;

	visibleCutscenes = true;
	currentDifficulty = 1;
	currentPlayerColour = 1;
	characterScene[0] = -1 ;

	currentGameScreen = SPLASH;
	currentMainMenuOption = START;
	currentOptionMenuOption = VOLUME;
	currentTrophyMenuOption = TROPHY1;
	EndGame = 0;

	// Stores colours that a user can choose from for the player
	playerColours = { Play::cCyan, Play::cMagenta, Play::cOrange };

	gameControls = { Play::KEY_A, Play::KEY_W, Play::KEY_D, Play::KEY_S };
	MainMenu = { " ", "NEW GAME", "OPTIONS", "TROPHIES", "EXIT" };
	OptionMenu = { "VOLUME", "CUTSCENES", "DIFFICULTY", "CONTROLS", "PLAYER" };
	EndMenu = { "PLAY AGAIN", "EXIT" };

	currentOptionValues = { " ", "ON", "EASY", "WASD", " " };
	gameDifficultyOptions = { "EASY", "NORMAL", "HARD" };

	itemsFile = "Data\\Textfiles\\ItemList.txt";

	// Reads and stores data related to game tips (tagline, description)
	LoadInfo("[Tip]", 5, tipCarousel, TipMap);
	// Reads and stores data related to game trophies (name, how it was achieved)
	LoadInfo("[Trophy]", TROPHY_BACK, TrophyMenu, TrophyMap, TrophyGet);

}

GameManager::~GameManager() {
	// Destructor
}

void GameManager::DrawDialogue() {

	int tempInt = currentNPCs->PrintDialogue(currentControls);

	if (Play::KeyPressed(gameControls[0]) && tempInt >= 1) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 1);
	}
	else if (Play::KeyPressed(gameControls[1]) && tempInt >= 2) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 2);
	}
	else if (Play::KeyPressed(gameControls[2]) && tempInt >= 3) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 3);
	}
	else if (Play::KeyPressed(gameControls[3]) && tempInt >= 4) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 4);
	}
	else if (Play::KeyPressed(Play::KEY_SPACE) && tempInt == 0) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 1);
	}

}

void GameManager::DrawInteraction (std::vector<std::string> interactionDesc, std::vector<std::string> interactionChoices) {

	Play::DrawRect({ DISPLAY_TILE, DISPLAY_HEIGHT * 0.5f }, { DISPLAY_WIDTH - DISPLAY_TILE, DISPLAY_HEIGHT - DISPLAY_TILE }, Play::cWhite, true);
	Play::DrawRect({ DISPLAY_TILE + DISPLAY_TILE * 0.25f, DISPLAY_HEIGHT * 0.5f + DISPLAY_TILE * 0.25f }, { DISPLAY_WIDTH - (DISPLAY_TILE + DISPLAY_TILE * 0.25f), DISPLAY_HEIGHT - (DISPLAY_TILE + DISPLAY_TILE * 0.25f) }, Play::cBlack, true);
	int a = int(interactionDesc.size());

	if (updateInteraction[0] == false) {

		for (int i = 0; i < a; i++) {
			if (i == 0) {

				Play::DrawFontText("32px", interactionDesc[i], { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - 3 * DISPLAY_TILE }, Play::CENTRE);

			}
			else {

				const char* charPtr = interactionDesc[i].c_str();
				Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (3 + i) * DISPLAY_TILE }, charPtr, Play::cWhite, true);
			}

		}

		Play::DrawDebugText({ 6 * DISPLAY_TILE, DISPLAY_HEIGHT - (4 + a) * DISPLAY_TILE }, "Press SPACE to continue", Play::cWhite, true);
		Play::DrawDebugText({ DISPLAY_WIDTH - (6 * DISPLAY_TILE), DISPLAY_HEIGHT - (4 + a) * DISPLAY_TILE }, "Press X to leave", Play::cWhite, true);

	}
	else {

		if (updateInteraction[1] == true) {
			srand(time(0));
			holdInteraction[0] = int(rand() % interactionChoices.size());
			holdInteraction[1] = currentLevel->GetEnemyChoice(characterScene[0]);
			updateInteraction[1] = false;

		}

		if (interactionDesc[0] == "[Coin Flip]" || interactionDesc[0] == "[Higher or Lower]") {

			std::string enemyStr;
			enemyStr = "The opponent chooses " + interactionChoices[holdInteraction[1]];
			const char* enemyPtr = enemyStr.c_str();
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (4 * DISPLAY_TILE) }, enemyPtr, Play::cWhite, true);
		}

		if (holdInteraction[2] != -1) {

			if (updateInteraction.size() != 7) {
				if (updateInteraction.size() == 6 && updateInteraction[4] == true && updateInteraction[5] == true) {
					updateInteraction[3] = true;
				}
				else {
					Play::DrawDebugText({ 6 * DISPLAY_TILE, DISPLAY_HEIGHT - (4 + a) * DISPLAY_TILE }, "Press SPACE to play again", Play::cWhite, true);
				}
			}
			else {
				if (updateInteraction[4] == true && updateInteraction[5] == true
					|| updateInteraction[6] == true && updateInteraction[5] == true
					|| updateInteraction[4] == true && updateInteraction[6] == true) {
					updateInteraction[3] = true;
				}
			}
			Play::DrawDebugText({ DISPLAY_WIDTH - (6 * DISPLAY_TILE), DISPLAY_HEIGHT - (4 + a) * DISPLAY_TILE }, "Press X to leave", Play::cWhite, true);
			int tempInt = updateInteraction.size() - 1;
			if (updateInteraction[tempInt] == true) {
				Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (6 * DISPLAY_TILE) }, "You win!", Play::cWhite, true);
			}
			else {
				Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (6 * DISPLAY_TILE) }, "You lost.", Play::cWhite, true);
			}

		}

	}

}

void GameManager::InteractionManager(std::vector<std::string> interactionDesc, std::vector<std::string> interactionChoices) {

	if (Play::KeyPressed(gameControls[0]) && interactionChoices.size() >= 1) {
		holdInteraction[2] = 0;
	}
	else if (Play::KeyPressed(gameControls[1]) && interactionChoices.size() >= 2) {
		holdInteraction[2] = 1;
	}
	else if (Play::KeyPressed(gameControls[2]) && interactionChoices.size() >= 3) {
		holdInteraction[2] = 2;
	}
	else if (Play::KeyPressed(gameControls[3]) && interactionChoices.size() >= 4) {
		holdInteraction[2] = 3;
	}

	if (holdInteraction[2] != -1) {

		if (interactionDesc[0] == "[Coin Flip]" || interactionDesc[0] == "[Higher or Lower]") {
			std::string tempStr;
			tempStr = "The result is " + interactionChoices[holdInteraction[0]];
			const char* tempPtr = tempStr.c_str();
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (3 * DISPLAY_TILE) }, tempPtr, Play::cWhite, true);
		}
		else {
			std::string enemyStr;
			enemyStr = "The opponent chose " + interactionChoices[holdInteraction[1]];
			const char* enemyPtr = enemyStr.c_str();
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (4 * DISPLAY_TILE) }, enemyPtr, Play::cWhite, true);
		}

		std::string playerStr;
		playerStr = "You chose " + interactionChoices[holdInteraction[2]];
		const char* playerPtr = playerStr.c_str();
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (5 * DISPLAY_TILE) }, playerPtr, Play::cWhite, true);
		bool result = false;

		if (interactionDesc[0] == "[Rock, Paper, Scissors]") {
			if (holdInteraction[2] == 0 && holdInteraction[1] == 2) {
				result = true;
			}
			else if (holdInteraction[2] == 2 && holdInteraction[1] == 1) {
				result = true;
			}
			else if (holdInteraction[2] == 1 && holdInteraction[1] == 0) {
				result = true;
			}
		}
		else if (interactionDesc[0] == "[Blue or Yellow]") {
			if (holdInteraction[2] == holdInteraction[1]) {
				result = true;
			}
		}
		else {
			if (holdInteraction[0] == holdInteraction[2]) {
				result = true;
			}
		}

		if (updateInteraction[2] == false) {
			updateInteraction.push_back(result);
			updateInteraction[2] = true;
		}

	}
	else {

		std::vector<std::string> ctrlArr;

		if (currentControls == true) {
			ctrlArr = { "A", "W", "D", "S" };
		}
		else {
			ctrlArr = { "LEFT", "UP", "RIGHT", "DOWN" };
		}
		for (int i = 0; i < interactionChoices.size(); i++) {

			const char* choicePtr = interactionChoices[i].c_str();
			const char* controlsPtr = ctrlArr[i].c_str();
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (5 + i) * DISPLAY_TILE }, choicePtr, Play::cWhite, true);
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f - DISPLAY_TILE * 2, DISPLAY_HEIGHT - (5 + i) * DISPLAY_TILE }, controlsPtr, Play::cWhite, true);

		}

	}

}

void GameManager::PlayGame(float elapsedTime) {
	int tempInt = -1;
	if (visibleCutscenes == false) {
		if (currentLevel->GetLevelIndex() == 0) {
			currentLevel->SetLevel(1);
		}
		else if (currentLevel->GetLevelIndex() == 7) {
			currentGameScreen = GAME_END;
			return;
		}
	}
	else {
		tempInt = currentLevel->GetLevelIndex();
	}
	std::vector<Play::Point2D> userPos = currentLevel->GetLevel();
	bool levelEnd = false;

	if (levelStart) {

		currentLevel->CreateObstacles();
		currentPlayer->SetExits(userPos);
		if (visibleCutscenes == true && (tempInt == 0 || tempInt == 7)) {
			userPos[1].y -= DISPLAY_TILE;
			currentNPCs->SetPosition(userPos[1]);
			currentNPCs->ResetDialogue(0, -1);
		}
		levelStart = false;

	}
	if (characterScene[0] != -1) {
		currentPlayer->DrawCharacter();
		std::vector<std::string> interactionDesc = currentLevel->GetEnemyInteraction(characterScene[0]);
		std::vector<std::string> interactionChoices = currentLevel->GetInteractionLimit(characterScene[0]);

		DrawInteraction(interactionDesc, interactionChoices);
		if (updateInteraction[0] == false) {
			if (Play::KeyPressed(Play::KEY_SPACE) && updateInteraction.size() != 7 && updateInteraction[3] == false) {
				updateInteraction[0] = true;
			}
		}
		else {
			InteractionManager(interactionDesc, interactionChoices);
			if (Play::KeyPressed(Play::KEY_SPACE) && updateInteraction.size() != 7 && updateInteraction[3] == false) {
				updateInteraction[1] = true;
				holdInteraction[2] = -1;
				updateInteraction[2] = false;
			}
		}
		if (Play::KeyPressed(Play::KEY_X)) {
			if (updateInteraction[3] == true) {
				currentLevel->ClearEnemy(characterScene[0]);
			}
			updateInteraction.clear();
			updateInteraction = { false, true, false, false };
			holdInteraction[2] = -1;
			characterScene[0] = -1;
			levelStart = true;

		}
	}
	else {
		if (tempInt == 0 || tempInt == 7) {
			currentNPCs->DrawCharacter();
		}
		if (currentNPCs->CheckCollision(currentPlayer->GetPosition())) {
			DrawDialogue();
		}
		else {
			currentPlayer->SetBoundaries(currentLevel->GetBoundaries());
			characterScene[0] = currentLevel->ManageEnemies(currentPlayer->GetPosition(), elapsedTime);
			levelEnd = currentPlayer->HandleControls(gameControls);
			if (tempInt == 0 || tempInt == 7) {
				levelEnd = currentNPCs->DialogueEnd();
			}
			levelStart = currentLevel->CheckTraps(currentPlayer->GetPosition());
		}
		if (Play::KeyPressed(Play::KEY_X)) {
			levelStart = true;
		}
	}
	currentPlayer->DrawCharacter();
	if (levelEnd) {
		currentLevel->ClearObstacles();
		currentLevel->SetLevel();
		if (currentLevel->GetLevelIndex() == (8)) {
			currentGameScreen = GAME_END;
			return;
		}
		if (tempInt + 1 == 7) {
			currentNPCs->ResetDialogue(7, -1);
		}
		levelStart = true;
		levelEnd = false;

	}

	screenTimer += elapsedTime;

}

void GameManager::LoadInfo(std::string keyword, int max, std::vector<std::string>& existingVector, std::map<std::string, std::vector<std::string>>& existingMap, std::vector<bool> myVector) {

	std::ifstream inFile(itemsFile);
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

void GameManager::DrawTexts(std::vector<std::string> tempStr) {

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

	int a = int(tempStr.size() - 1);
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

					DrawTexts(TrophyMap.at(myVector[currentTrophyMenuOption]));

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
		currentPlayer->DrawCharacter();

	}

	if (currentGameScreen == TROPHY_MENU || currentGameScreen == OPTION_MENU) {

		Play::DrawFontText("32px", "BACK", { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) + j }, Play::LEFT);

	}

}

void GameManager::ScreenTips(float elapsedTime) {

	if (screenTimer < 6.0f) {

		DrawTexts(TipMap.at(tipCarousel[carouselIndex]));
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

int GameManager::MenuInteraction(int optionValue, int maxValue, GameScreen newScreen) {

	float j = 50 * (4 * 0.5f);

	if (Play::KeyPressed(gameControls[1]) && optionValue != 0) {

		optionValue--;

	}
	else if (Play::KeyPressed(gameControls[3]) && optionValue != maxValue) {

		optionValue++;

	}
	else if (Play::KeyPressed(Play::KEY_SPACE)) {

		if (currentGameScreen == GAME_END) {
			if (optionValue == 1) {
				newScreen = GAME_EXIT;
			}
		}

		if (newScreen == GAME_EXIT) {

			delete currentLevel;
			delete currentPlayer;
			delete currentNPCs;
			currentLevel = nullptr;
			currentPlayer = nullptr;
			currentNPCs = nullptr;
			inactiveGame = true;

		}
		else {

			playerOldPos = currentPlayer->GetPosition();
			currentGameScreen = newScreen;

		}
	
	}
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH * 0.5f) - (DISPLAY_TILE * 0.5f), (DISPLAY_HEIGHT * 0.5f) + j - (optionValue * 50) }, Play::CENTRE);
	return optionValue;

}

bool GameManager::ScreenUpdate(float elapsedTime) {

	switch (currentGameScreen) {
	case SPLASH:

		if (screenTimer < SPLASH_TIMELIMIT) {

			Play::ClearDrawingBuffer(Play::cBlack);
			Play::DrawCircle({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f }, float(DISPLAY_TILE * screenTimer), Play::cOrange);
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

			currentMainMenuOption = MainMenuScreen(MenuInteraction(int(CONTINUE), int(EXIT), PLAY));
			levelStart = false;

			break;
		case START:
			
			currentMainMenuOption = MainMenuScreen(MenuInteraction(int(START), int(EXIT), PLAY));
			levelStart = true;
			updateInteraction.clear();
			updateInteraction = { false, true, false, false };
			holdInteraction[2] = -1;
			characterScene[0] = -1;
			screenTimer = 0.0f;

			break;
		case OPTIONS:
			currentMainMenuOption = MainMenuScreen(MenuInteraction(int(OPTIONS), int(EXIT), OPTION_MENU));
			
			break;
		case TROPHIES:
			currentMainMenuOption = MainMenuScreen(MenuInteraction(int(TROPHIES), int(EXIT), TROPHY_MENU));

			break;
		case EXIT:
			currentMainMenuOption = MainMenuScreen(MenuInteraction(int(EXIT), int(EXIT), GAME_EXIT));

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

				if (currentDifficulty < gameDifficultyOptions.size()) {

					currentDifficulty++;

				}
				else {

					currentDifficulty = 1;

				}

				currentLevel->SetMode(currentDifficulty);
				currentOptionValues[DIFFICULTY] = gameDifficultyOptions[currentDifficulty - 1];

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

				currentPlayer->SetPosition(playerOldPos);
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

		}

		Play::PresentDrawingBuffer();
		break;
	case GAME_END:
		Play::ClearDrawingBuffer(Play::cBlack);

		Play::DrawFontText("72px", "GAME OVER", { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f + 200 }, Play::CENTRE);
		DrawOptions(EndMenu);
		EndGame = MenuInteraction(EndGame, 1, MAIN_MENU);

		TrophyGet[currentDifficulty - 1] = true;
		if (screenTimer < 60) {
			TrophyGet[TROPHY5] = true;
		}
		else if (screenTimer >= 600) {
			TrophyGet[TROPHY4] = true;
		}
		MainMenu[0] = " ";

		Play::PresentDrawingBuffer();
		break;
	default:
		// Current game state is invalid, directly exit game
		inactiveGame = true;

	}

	return inactiveGame;

}