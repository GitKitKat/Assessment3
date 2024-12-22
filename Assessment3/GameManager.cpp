/* Includes:
This file's header */
#include "GameManager.h"

/* Standard header files */
#include <fstream>
#include <cstdlib>

GameManager::GameManager() {
	/* Definitions */
	//
	screenTimer = 0.0f;
	inactiveGame = false; 
	gameVolume = 0.5f; // starting audio volume: 50% 
	levelStart = true;
	currentControls = true; // starting inputs: W, A, S, D keys
	updateEncounter = { false, false, false, false }; // whether: the encounter has started once; the opponent has made a move; player has made a move; player won the encounter 
	holdEncounterChoices = { 0, 0, -1 }; // holds: random result of an encounter; opponent's choice; player's choice
	visibleCutscenes = true; // whether NPCs should be encountered
	currentDifficulty = 1; // starting difficulty: EASY
	currentPlayerColour = 0; 
	characterScene[0] = -1 ; // the index of the encountered enemy. -1 if no enemy is encountered
	currentGameScreen = SPLASH; 
	currentMainMenuOption = START; 
	carouselIndex = 0;
	currentOptionMenuOption = VOLUME; 
	currentTrophyMenuOption = 0; 
	currentEndGameOption = 0;
	playerColours = { Play::cBlue, Play::cBlack, Play::cWhite, Play::cCyan, Play::cOrange };
	gameControls = { Play::KEY_A, Play::KEY_W, Play::KEY_D, Play::KEY_S };
	MainMenu = { " ", "NEW GAME", "OPTIONS", "TROPHIES", "EXIT" };
	OptionMenu = { "VOLUME", "CUTSCENES", "DIFFICULTY", "CONTROLS", "PLAYER" };
	EndMenu = { "MAIN MENU", "EXIT" };
	currentOptionValues = { std::to_string(int(gameVolume * 100)) + "%", "ON", "EASY", "WASD", " " };
	gameDifficultyOptions = { "EASY", "NORMAL", "HARD" };
	itemsFile = "Data\\Textfiles\\ItemList.txt";
	trophyFile = "Data\\Textfiles\\TrophyList.txt";

	// Reads and stores data related to game tips. Number of tips loaded form file: 8
	LoadInfo("[Tip]", itemsFile, 8, tipCarousel, TipMap);
	// Reads and stores data related to game trophies. Number of trophies loaded from file: 8
	LoadInfo("[Trophy]", trophyFile, 8, TrophyMenu, TrophyMap, TrophyGet);

}

GameManager::~GameManager() {
	// Destructor
}

void GameManager::DrawDialogue() {

	int tempInt = currentNPCs->PrintDialogue(currentControls);
	// Player chooses:
	// (excluding CONTINUE, next dialogue depends on the choice made)
	// the first option (LEFT button)
	if (Play::KeyPressed(gameControls[0]) && tempInt >= 1) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 1);
	}
	// the second option (UP button)
	else if (Play::KeyPressed(gameControls[1]) && tempInt >= 2) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 2);
	}
	// the third option (RIGHT button)
	else if (Play::KeyPressed(gameControls[2]) && tempInt >= 3) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 3);
	}
	// the fourth option (DOWN button)
	else if (Play::KeyPressed(gameControls[3]) && tempInt >= 4) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 4);
	} 
	// option to CONTINUE (next dialogue will always be the same) 
	else if (Play::KeyPressed(Play::KEY_SPACE) && tempInt == 0) {
		currentNPCs->ResetDialogue(currentLevel->GetLevelIndex(), 1);
	}

}

void GameManager::DrawEncounter (std::vector<std::string> encounterDesc, std::vector<std::string> encounterChoices) {
	// Draws a black rectangle with a white border 
	Play::DrawRect({ DISPLAY_TILE, DISPLAY_HEIGHT * 0.5f }, { DISPLAY_WIDTH - DISPLAY_TILE, DISPLAY_HEIGHT - DISPLAY_TILE }, Play::cWhite, true);
	Play::DrawRect({ DISPLAY_TILE + DISPLAY_TILE * 0.25f, DISPLAY_HEIGHT * 0.5f + DISPLAY_TILE * 0.25f }, { DISPLAY_WIDTH - (DISPLAY_TILE + DISPLAY_TILE * 0.25f), DISPLAY_HEIGHT - (DISPLAY_TILE + DISPLAY_TILE * 0.25f) }, Play::cBlack, true);

	if (!updateEncounter[0]) {

		for (int i = 0; i < encounterDesc.size(); i++) {
			if (i == 0) { // Draws the name of the encounter
				Play::DrawFontText("32px", encounterDesc[i], { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - 3 * DISPLAY_TILE }, Play::CENTRE);
			}
			else { // Draws the description of the encounter
				Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (3 + i) * DISPLAY_TILE }, encounterDesc[i].c_str(), Play::cWhite, true);
			}

		}
		// Draws available general controls (SPACE changes to the next dialogue, X exits the dialogue encounter) 
		Play::DrawDebugText({ 6 * DISPLAY_TILE, DISPLAY_HEIGHT - (4 + int(encounterDesc.size())) * DISPLAY_TILE }, "Press SPACE to continue", Play::cCyan, true);
		Play::DrawDebugText( { DISPLAY_WIDTH - (6 * DISPLAY_TILE), DISPLAY_HEIGHT - (4 + int(encounterDesc.size())) * DISPLAY_TILE }, "Press X to leave", Play::cCyan, true);
	}
	else {

		if (!updateEncounter[1]) { // If the opponent has not made a move, their move (and the result) are randomly generated
			holdEncounterChoices[0] = int(rand() % encounterChoices.size());
			holdEncounterChoices[1] = currentLevel->GetEnemyChoice(characterScene[0]);
			updateEncounter[1] = true;

		}

		if (encounterDesc[0] == "[Coin Flip]" || encounterDesc[0] == "[Higher or Lower]") { // Draws the enemy's choice before a result is shown
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (4 * DISPLAY_TILE) }, std::string("The opponent chooses " + encounterChoices[holdEncounterChoices[1]]).c_str(), Play::cWhite, true);
		}

		if (holdEncounterChoices[2] != -1) { // Handles the display of encounter options after the player makes a choice (displays: continue; exit)

			if (updateEncounter.size() != 7) { // Checks if three rounds of the encounter have occurred
				if (updateEncounter.size() == 6 && updateEncounter[4] && updateEncounter[5]) {
					updateEncounter[3] = true;
				}
				else { 
					Play::DrawDebugText({ 6 * DISPLAY_TILE, DISPLAY_HEIGHT - (4 + int(encounterDesc.size())) * DISPLAY_TILE }, "Press SPACE to play again", Play::cCyan, true);
				}
			}
			else { // Checks if the player has won two or more rounds of the encounter 
				if (updateEncounter[4] && updateEncounter[5]
					|| updateEncounter[6] && updateEncounter[5]
					|| updateEncounter[4] && updateEncounter[6]) {
					updateEncounter[3] = true;
				}
			}
			Play::DrawDebugText({ DISPLAY_WIDTH - (6 * DISPLAY_TILE), DISPLAY_HEIGHT - (4 + int(encounterDesc.size())) * DISPLAY_TILE }, "Press X to leave", Play::cCyan, true);
			int tempInt = updateEncounter.size() - 1;
			if (updateEncounter[tempInt]) { // Draws the result of a round (player win or loss)
				Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (6 * DISPLAY_TILE) }, "You win!", Play::cGreen, true);
			}
			else {
				Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (6 * DISPLAY_TILE) }, "You lost.", Play::cRed, true);
			}

		}

	}

}

void GameManager::EncounterManager(std::vector<std::string> encounterDesc, std::vector<std::string> encounterChoices) {
	// Handles the player's input in an encounter
	if (Play::KeyPressed(gameControls[0]) && encounterChoices.size() >= 1) { // First option (LEFT or A key)
		holdEncounterChoices[2] = 0;
	}
	else if (Play::KeyPressed(gameControls[1]) && encounterChoices.size() >= 2) { // Second option (UP or W key)
		holdEncounterChoices[2] = 1;
	}
	else if (Play::KeyPressed(gameControls[2]) && encounterChoices.size() >= 3) { // Third option (RIGHT or D key)
		holdEncounterChoices[2] = 2;
	}
	else if (Play::KeyPressed(gameControls[3]) && encounterChoices.size() >= 4) { // Fourth option (DOWN or S key)
		holdEncounterChoices[2] = 3;
	}

	if (holdEncounterChoices[2] != -1) { // Handles the display of encounter results 
		if (encounterDesc[0] == "[Coin Flip]" || encounterDesc[0] == "[Higher or Lower]") { // Draws the results of an encounter if the enemy's choice was previously drawn
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (3 * DISPLAY_TILE) }, std::string("The result is " + encounterChoices[holdEncounterChoices[0]]).c_str(), Play::cWhite, true);
		}
		else { // Draws the enemy's choice 
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (4 * DISPLAY_TILE) }, std::string("The opponent chose " + encounterChoices[holdEncounterChoices[1]]).c_str(), Play::cWhite, true);
		}
		// Draws the player's choice
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (5 * DISPLAY_TILE) }, std::string("You chose " + encounterChoices[holdEncounterChoices[2]]).c_str(), Play::cWhite, true);
		// Stores the result of an interaction (whether the player won or not)
		bool result = false;

		if (encounterDesc[0] == "[Rock, Paper, Scissors]") {
			if (holdEncounterChoices[2] == 0 && holdEncounterChoices[1] == 2) { // Player chooses Rock and enemy chooses Scissors
				result = true;
			}
			else if (holdEncounterChoices[2] == 2 && holdEncounterChoices[1] == 1) { // Player chooses Scissors and enemy chooses Paper
				result = true;
			}
			else if (holdEncounterChoices[2] == 1 && holdEncounterChoices[1] == 0) { // Player chooses Paper and enemy chooses Rock
				result = true;
			}
		}
		else if (encounterDesc[0] == "[Blue or Yellow]") {
			if (holdEncounterChoices[2] == holdEncounterChoices[1]) { // Player and enemy choose the same colour
				result = true;
			}
		}
		else {
			if (holdEncounterChoices[0] == holdEncounterChoices[2]) { // For encounters where the player's choice should match the random result
				result = true;
			}
		}

		if (!updateEncounter[2]) { // Updates if the player has made a move
			updateEncounter.push_back(result);
			updateEncounter[2] = true;
		}

	}
	else {
		// Stores the currently active controls as separate strings
		std::vector<std::string> ctrlArr;
		if (currentControls) { // If true, current inputs are W, A, S, D keys
			ctrlArr = { "A", "W", "D", "S" };
		}
		else {
			ctrlArr = { "LEFT", "UP", "RIGHT", "DOWN" };
		}
		for (int i = 0; i < encounterChoices.size(); i++) { // Draws the available choices in an encounter and their corresponding input key

			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (5 + i) * DISPLAY_TILE }, encounterChoices[i].c_str(), Play::cCyan, true);
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f - DISPLAY_TILE * 2, DISPLAY_HEIGHT - (5 + i) * DISPLAY_TILE }, ctrlArr[i].c_str(), Play::cCyan, true);

		}

	}

}

void GameManager::PlayGame(float elapsedTime) {
	
	if (!visibleCutscenes) { // If cutscenes are off
		if (currentLevel->GetLevelIndex() == 0) { // Skips the beginning NPC level
			currentLevel->SetLevel();
		}
		else if (currentLevel->GetLevelIndex() == 7) { // Skips the ending NPC level 
			currentGameScreen = GAME_END;
			Play::PlayAudio("end");
			return;
		}
	}
	// Stores the ID of the current level
	int tempInt = currentLevel->GetLevelIndex();
	// Stores the positions of the level's entrance and exit
	std::vector<Play::Point2D> startPos = currentLevel->GetLevel();
	// Stores a value (whether the player has reached the level's exit or not)
	bool levelEnd = false;

	if (levelStart) { // Resets the currently loaded level (cleared enemies, enemy positions, player position)
		currentLevel->CreateEnemies();
		currentPlayer->SetExits(startPos);
		if (visibleCutscenes && (tempInt == 0 || tempInt == 7)) { // If cutscenes are enabled, restarts the NPC's dialogue
			currentNPCs->SetPosition(startPos[1]);
			if (currentNPCs->NextDialogue()) {
				currentNPCs->ResetDialogue(tempInt, 1);
			}
		}
		if (currentLevel->GetLevelIndex() == 15 && currentDifficulty == 3) {
			TrophyGet[3] = true; // Claim achievement: 'enter the level made with a binary message' (flipped 5th floor, HARD mode)
		}
		levelStart = false;

	}
	if (characterScene[0] != -1) { // Checks if an enemy is encountered
		// Stores the description of the enemy encounter
		std::vector<std::string> encounterDesc = currentLevel->GetEnemyEncounter(characterScene[0]);
		// Stores the available choices of an enemy encounter
		std::vector<std::string> encounterChoices = currentLevel->GetEncounterChoices(characterScene[0]);
		DrawEncounter(encounterDesc, encounterChoices); // Draws the encounter on the game window
		if (!updateEncounter[0]) { // Checks whether the encounter has started (if true, draws the encounter description)
			if (Play::KeyPressed(Play::KEY_SPACE) && updateEncounter.size() != 7 && !updateEncounter[3]) {
				updateEncounter[0] = true;
			}
		}
		else { 
			EncounterManager(encounterDesc, encounterChoices); // Handles player and enemy choices during the encounter
			if (Play::KeyPressed(Play::KEY_SPACE) && updateEncounter.size() != 7 && !updateEncounter[3]) { // Starts a new round if three rounds have not been played
				updateEncounter[1] = false;
				holdEncounterChoices[2] = -1;
				updateEncounter[2] = false;
			}
		}
		if (Play::KeyPressed(Play::KEY_X)) { // Leaves the encounter
			if (updateEncounter[3]) { // Clears the enemy if the player won the encounter
				currentLevel->ClearEnemy(characterScene[0]); 
			}
			else { // Resets the player's position if they lost the encounter
				levelStart = true;
			} 
			updateEncounter.clear(); 
			updateEncounter = { false, false, false, false };
			holdEncounterChoices[2] = -1;
			characterScene[0] = -1; 

		}
	}
	else {
		if (visibleCutscenes && tempInt == 0 || tempInt == 7) { // Checks if the current level is a cutscene level
			currentNPCs->DrawCharacter();
			currentPlayer->DrawCharacter();
			if (currentNPCs->CheckCollision(currentPlayer->GetPosition())) { // Draws NPC dialogue if they interact with a player
				DrawDialogue();
			}
			else { // Handles player inputs as character movement if a dialogue is not ongoing
				levelEnd = currentPlayer->HandleControls(gameControls);
			}
			levelEnd = currentNPCs->DialogueEnd(); // Ends the level if the NPC's dialogue is complete
		}
		else { // Occurs if the current level is not a cutscene level
			characterScene[0] = currentLevel->ManageEnemies(currentPlayer->GetPosition(), elapsedTime); // Checks whether the player has encountered an enemy
			if (characterScene[0] != -1) {
				Play::PlayAudio("encounter");
			}
			levelStart = currentLevel->CheckTraps(currentPlayer->GetPosition()); // Checks whether the player has encountered a trap or key
			levelEnd = currentPlayer->HandleControls(gameControls); // Checks whether the player has encountered the exit
			currentPlayer->DrawCharacter();
		}
		currentPlayer->SetBoundaries(currentLevel->GetBoundaries());
		if (Play::KeyPressed(Play::KEY_X)) { // Resets the player's position
			levelStart = true;
		}
	}
	if (levelEnd) { // Occurs if the player has cleared a level
		currentLevel->DeleteEnemies();
		currentLevel->SetLevel();
		if (currentLevel->GetLevelIndex() == 7 && visibleCutscenes) { // Checks if the second NPC level should be loaded
			currentNPCs->ResetDialogue(7, -1); // Loads the necessary dialogue before the level is drawn
		}
		if (currentLevel->GetLevelIndex() == 8) {
			currentGameScreen = GAME_END; // The last level in every mode is level 7 (NPC encounter) so the 'next level' is a game over 
			Play::PlayAudio("end");
			return;
		}
		levelStart = true;
		levelEnd = false;

	}
	screenTimer += elapsedTime;

}

void GameManager::PrintInfo(std::string keyword, std::string file, std::vector<std::string> itemName, std::map<std::string, std::vector<std::string>> itemMap, std::vector<bool> itemGotten) {
	// Opens the file to be written in
	std::ofstream outFile(file);
	if (!outFile.is_open()) { // Returns if the file could not be opened
		return;
	}
	else {
		for (int i = 0; i < itemMap.size(); i++) { // Prints all stored items to the file

			outFile << keyword << std::endl;
			outFile << "(Name)" << itemName[i] << std::endl;
			for (std::string j : itemMap.at(itemName[i])) {
				outFile << "(Desc)" << j << std::endl;
			}
			if (itemGotten[i]) {
				outFile << "(Get)" << "T" << std::endl;
			}
			else {
				outFile << "(Get)" << "F" << std::endl;
			}
			outFile << "[End]" << std::endl; // End of a single item
			outFile << "\n";

		}

	}
	outFile.close(); // Closes the file

}

void GameManager::LoadInfo(std::string keyword, std::string file, int max, std::vector<std::string>& existingVector, std::map<std::string, std::vector<std::string>>& existingMap, std::vector<bool> myVector) {
	// Opens the file for reading
	std::ifstream inFile(file);
	// Stores each line of the opened file as a new string
	std::string newLine;
	// Stores the name or title of the item to be loaded and stored
	std::string holdItemName;
	// Stores the description of the item to be loaded and stored
	std::vector<std::string> holdItemDesc;
	// Whether the item was successfully found
	bool itemFound = false;
	// Stores the number of successfully found and stored items
	int min = 0;

	if (!inFile.is_open()) { // Returns if the file could not be opened
		return;
	}
	else {
		// This loop reads and relays data from the passed text file
		while (std::getline(inFile, newLine)) {

			if (newLine.rfind(keyword, 0) == 0) { // Checks whether the group of items was found (e.g. if the currently read item falls under 'Trophies')
				itemFound = true;
			}

			if (itemFound) {

				if (newLine.rfind("(Name)", 0) == 0) { // Temporarily stores the name of the found item

					holdItemName = newLine.substr(6);
					existingVector.push_back(holdItemName);

				}
				else if (newLine.rfind("(Desc)", 0) == 0) { // Temporarily stores the description of the found item
					holdItemDesc.push_back(newLine.substr(6));
				}
				else if (newLine.rfind("(Get)", 0) == 0) { // If the item has a condition to be visible, checks the condition's status 

					if (newLine.substr(5) == "F") {
						myVector.push_back(false);
					}
					else {
						myVector.push_back(true);
					}

				}
				else if (newLine == "[End]") { // Ends the reading for the current item

					existingMap.insert({ holdItemName, holdItemDesc }); // Stores the loaded item
					holdItemDesc.clear();
					min++;
					if (min == max) { // Ends the reading for the file
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
	inFile.close(); // Closes the file

}

void GameManager::DrawTexts(std::vector<std::string> tempStr) {
	// Draws the current tip (title, description) and its position in the vector of tips
	if (currentGameScreen == MAIN_MENU) {

		for (int a = 0; a < tempStr.size() - 1; a++) { // Draws the tip's title
			Play::DrawFontText("32px", tempStr[a], { DISPLAY_TILE * 2, (DISPLAY_HEIGHT * 0.5f) - ((2 + a) * DISPLAY_TILE) }, Play::LEFT);
		} // Draws the tip's description and number
		Play::DrawDebugText({ DISPLAY_TILE * 2,  (DISPLAY_HEIGHT * 0.5f) - ((tempStr.size() + 1) * DISPLAY_TILE) }, tempStr[tempStr.size() - 1].c_str(), Play::cWhite, false);
		Play::DrawDebugText({ DISPLAY_TILE * 2,  (DISPLAY_HEIGHT * 0.5f) - ((tempStr.size() + 2) * DISPLAY_TILE) }, std::string("Tip " + std::to_string(carouselIndex + 1) + " of " + std::to_string(tipCarousel.size())).c_str(), Play::cWhite, false);
	}
	// Draws the currently selected trophy (title, description) if it has been achieved
	else if (currentGameScreen == TROPHY_MENU) {

		for (int a = 0; a < tempStr.size() - 1; a++) { // Draws the trophy's title
			Play::DrawFontText("32px", tempStr[a], { DISPLAY_TILE * 2, (DISPLAY_HEIGHT * 0.5f) - (a * DISPLAY_TILE) }, Play::LEFT);
		} // Draws the trophy's description
		Play::DrawDebugText({ DISPLAY_TILE * 2,  (DISPLAY_HEIGHT * 0.5f) - ((tempStr.size() - 1 ) * DISPLAY_TILE) }, tempStr[tempStr.size() - 1].c_str(), Play::cWhite, false);
	}

}

void GameManager::DrawOptions(std::vector<std::string> myVector) {

	for (int i = 0; i < myVector.size(); i++) {
		if (currentGameScreen == TROPHY_MENU) {

			if (!TrophyGet[i]) { // Hides the trophy name if it has not been achieved
				myVector[i] = "???";
			} // Checks whether the currently selected option is a trophy or the BACK option and draws the trophy details if it has been achieved
			else if (currentTrophyMenuOption < TrophyMap.size() && TrophyGet[currentTrophyMenuOption]) {
				DrawTexts(TrophyMap.at(myVector[currentTrophyMenuOption]));
			}

		}
		else if (currentGameScreen == OPTION_MENU) { // Draws the current control options (e.g. displays EASY if current difficulty is 1)
			Play::DrawFontText("32px", currentOptionValues[i], { (DISPLAY_WIDTH * 0.5f) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT * 0.5f) + ((2 * DISPLAY_TILE) - (i * DISPLAY_TILE)) }, Play::LEFT);
		} 
		Play::DrawFontText("32px", myVector[i], { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) + ((2 * DISPLAY_TILE) - (i * DISPLAY_TILE)) }, Play::LEFT);

	}

	if (currentGameScreen == OPTION_MENU) { // Draws the player on the OPTIONS screen
		// Stores a value relative to the placement of the PLAYER colour control
		Play::Point2D tempPos = { (DISPLAY_WIDTH * 0.5f) + (DISPLAY_TILE * 3), (DISPLAY_HEIGHT * 0.5f) + DISPLAY_TILE * (- PLAYER + 1.5f) };
		Play::DrawRect({ tempPos.x - 1, tempPos.y - 1 }, { tempPos.x + DISPLAY_TILE, tempPos.y + DISPLAY_TILE }, Play::cWhite, false);
		currentPlayer->SetPosition(tempPos);
		currentPlayer->DrawCharacter();

	}

	if (currentGameScreen == TROPHY_MENU || currentGameScreen == OPTION_MENU) { // Draws a BACK (return to MAIN_MENU) option 
		Play::DrawFontText("32px", "BACK", { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) + (2 * DISPLAY_TILE) - (myVector.size() * DISPLAY_TILE) }, Play::LEFT);
	}

}

void GameManager::ScreenTips(float elapsedTime) {

	if (screenTimer < 7.0f) { // Current tip is changed after 7 seconds
		screenTimer += elapsedTime;

	} else {
		if (carouselIndex < TipMap.size() - 1) { // Next game window update will display the next tip
			carouselIndex++;
		}
		else {
			carouselIndex = 0; // Resets to the first tip
		}
		screenTimer = 0.0f;
	}
	DrawTexts(TipMap.at(tipCarousel[carouselIndex]));

}

int GameManager::MenuEncounter(int optionValue, int maxValue) { 
	// Draws an icon next to the currently selected option
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH * 0.5f) - (DISPLAY_TILE * 0.5f), (DISPLAY_HEIGHT * 0.5f) + (2 * DISPLAY_TILE) - (optionValue * 50) }, Play::CENTRE);
	if (Play::KeyPressed(gameControls[1]) && optionValue != 0) {
		optionValue--;
	}
	else if (Play::KeyPressed(gameControls[3]) && optionValue != maxValue) {
		optionValue++;
	}
	return optionValue;

}

int GameManager::MenuEncounter(int optionValue, int maxValue, GameScreen newScreen) {

	if (Play::KeyPressed(gameControls[1]) && optionValue != 0) { // The next option (moving up) can be selected if it exists
		if (currentGameScreen == MAIN_MENU) { 
			if (!(optionValue == 1 && MainMenu[0] == " ")) { // Only allows the user to access the 'CONTINUE' game option if a game was previously started 
				optionValue--;
			}
		}
		else {
			optionValue--;
		}

	}
	else if (Play::KeyPressed(gameControls[3]) && optionValue != maxValue) { // The next option (moving down) can be selected if it exists 
		optionValue++;
	}
	else if (Play::KeyPressed(Play::KEY_SPACE)) {

		if (currentGameScreen == GAME_END) { 
			if (optionValue == 1) {
				newScreen = GAME_EXIT;
			}
		}

		if (newScreen == GAME_EXIT) { // Frees up memory before closing the game window

			delete currentLevel;
			delete currentPlayer;
			delete currentNPCs;
			currentLevel = nullptr;
			currentPlayer = nullptr;
			currentNPCs = nullptr;
			inactiveGame = true;
			Play::StopAudio("menu");

		}
		else {

			playerOldPos = currentPlayer->GetPosition(); // Returns the player to the original position
			if (newScreen == PLAY) { // Starts anew or continues an ongoing game
				Play::StopAudio("menu");
				updateEncounter.clear();
				updateEncounter = { false, false, false, false }; 
				holdEncounterChoices[2] = -1;
				characterScene[0] = -1;
				if (currentMainMenuOption == START) { // Starts a new game 
					currentLevel->SetLevel(0);
					currentLevel->DeleteEnemies();
					currentNPCs->ResetDialogue(0, -1);
					screenTimer = 0.0f;
				}
			}
			if (newScreen == MAIN_MENU) {
				Play::Audio::StartSound("menu", true, gameVolume);
			}

		}
		currentGameScreen = newScreen;
	
	}
	Play::DrawFontText("72px", "+", { (DISPLAY_WIDTH * 0.5f) - (DISPLAY_TILE * 0.5f), (DISPLAY_HEIGHT * 0.5f) + (2 * DISPLAY_TILE) - (optionValue * 50)}, Play::CENTRE);
	return optionValue;

}

bool GameManager::ScreenUpdate(float elapsedTime) {

	switch (currentGameScreen) {
	case SPLASH:
		// For the number of seconds in SPLASH_TIMELIMIT, displays the opening screen
		if (screenTimer < SPLASH_TIMELIMIT) {

			Play::ClearDrawingBuffer(Play::cBlack); // Prints the game title and a circle
			Play::DrawCircle({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f }, float(DISPLAY_TILE * screenTimer), Play::cOrange);
			Play::DrawFontText("72px", "THE REQUEST", { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f }, Play::CENTRE);
			Play::PresentDrawingBuffer();
			screenTimer += elapsedTime;
		}
		else {
			// Changes to the MAIN_MENU and resets the screen timer
			currentGameScreen = MAIN_MENU;
			screenTimer = 0.0f;
			Play::Audio::StartSound("menu", true, gameVolume);

		}
		break;
	case MAIN_MENU:
		// Displays the MAIN screen. Option selection is handled in each case
		Play::ClearDrawingBuffer(Play::cBlack);
		ScreenTips(elapsedTime);
		DrawOptions(MainMenu);
		
		switch (currentMainMenuOption) {
		case CONTINUE: // Continues an ongoing game

			currentMainMenuOption = MainMenuScreen(MenuEncounter(int(CONTINUE), int(EXIT), PLAY)); 
			levelStart = false;

			break;
		case START: // Starts a new game
			
			currentMainMenuOption = MainMenuScreen(MenuEncounter(int(START), int(EXIT), PLAY));
			currentLevel->SetMode(currentDifficulty);
			levelStart = true;

			break;
		case OPTIONS: // Displays the current (changable) game controls
			currentMainMenuOption = MainMenuScreen(MenuEncounter(int(OPTIONS), int(EXIT), OPTION_MENU));
			break;
		case TROPHIES: // Displays the list of obtainable trophies
			currentMainMenuOption = MainMenuScreen(MenuEncounter(int(TROPHIES), int(EXIT), TROPHY_MENU));
			break;
		case EXIT: // Closes the game window
			currentMainMenuOption = MainMenuScreen(MenuEncounter(int(EXIT), int(EXIT), GAME_EXIT));
			break;
		}
		Play::PresentDrawingBuffer();
		break;
	case OPTION_MENU:
		// Displays the OPTIONS menu, list of game controls and the current state of those controls
		Play::ClearDrawingBuffer(Play::cBlack);
		Play::DrawFontText("64px", "OPTIONS", { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f + 200 }, Play::CENTRE);
		DrawOptions(OptionMenu);
		currentOptionMenuOption = OptionScreen(MenuEncounter(int(currentOptionMenuOption), int(OPTION_BACK))); // Handles option selection

		switch (currentOptionMenuOption) {
		case VOLUME:

			if (Play::KeyPressed(Play::KEY_SPACE)) { // Changes the volume of the MAIN_MENU's audio track

				Play::StopAudio("menu");
				if (gameVolume > 0.25f) {
					gameVolume -= 0.25f;

				}
				else {
					gameVolume = 1.0f;
				}
				currentOptionValues[VOLUME] = std::to_string(int(gameVolume * 100)) + "%";
				Play::Audio::StartSound("menu", true, gameVolume);

			}
			break;
		case CUTSCENES:

			if (Play::KeyPressed(Play::KEY_SPACE)) { // Determines whether NPC encounters occur (if false, NPCs will not be encountered)
				visibleCutscenes = !visibleCutscenes;
				if (visibleCutscenes) {
					currentOptionValues[CUTSCENES] = "ON";
				}
				else {
					currentOptionValues[CUTSCENES] = "OFF";
				}
				MainMenu[0] = " "; // Removes the option to 'CONTINUE' an ongoing game
			}
			break;
		case DIFFICULTY:

			if (Play::KeyPressed(Play::KEY_SPACE)) { // Changes game difficulty between EASY (1), NORMAL (2), and HARD (3)
				if (currentDifficulty < gameDifficultyOptions.size()) {
					currentDifficulty++;
				}
				else {
					currentDifficulty = 1;
				}
				currentLevel->SetMode(currentDifficulty);
				currentOptionValues[DIFFICULTY] = gameDifficultyOptions[currentDifficulty - 1];
				MainMenu[0] = " "; 
			}
			break;
		case CONTROLS:

			if (Play::KeyPressed(Play::KEY_SPACE)) { // Changes input controls between W, A, S, D and arrow keys
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
				if (currentPlayerColour < playerColours.size() - 1) { // Changes the player's colour (cycles through a vector storing possible colours)
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

				currentPlayer->SetPosition(playerOldPos); // Resets the player's position
				currentGameScreen = MAIN_MENU;

			}
			break;

		}
		Play::PresentDrawingBuffer();
		break;
	case TROPHY_MENU:
		// Draws the TROPHY screen (list of trophies and (if they were achieved) their names, titles, & descriptions)
		Play::ClearDrawingBuffer(Play::cBlack);
		Play::DrawFontText("72px", "TROPHIES", { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f + 200 }, Play::CENTRE); 
		DrawOptions(TrophyMenu);
		currentTrophyMenuOption = MenuEncounter(int(currentTrophyMenuOption), int(TrophyMap.size())); // Handles trophy selection
		if (currentTrophyMenuOption == TrophyMap.size()) { 
			if (Play::KeyPressed(Play::KEY_SPACE)) { // Returns the user to the main menu if the BACK option is selected 
				currentGameScreen = MAIN_MENU;
			}
		}
		Play::PresentDrawingBuffer();
		break;
	case PLAY:

		PlayGame(elapsedTime); // Handles the actual game
		if (Play::KeyPressed(Play::KEY_ESCAPE)) { // Returns the user to the main menu
			Play::ClearDrawingBuffer(Play::cBlack);
			MainMenu[0] = "CONTINUE"; // Adds the option to continue the currently ongoing game
			currentGameScreen = MAIN_MENU;
			Play::Audio::StartSound("menu", true, gameVolume);
		}
		Play::PresentDrawingBuffer();
		break;
	case GAME_END:
		// Draws the GAME_OVER screen and options for further action (return to MAIN_MENU or quit)
		Play::ClearDrawingBuffer(Play::cBlack);
		Play::DrawFontText("64px", "You cleared the request!", { DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) + (5 * DISPLAY_TILE) }, Play::CENTRE);
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, (DISPLAY_HEIGHT * 0.5f) + (3.5f * DISPLAY_TILE) }, "Select MAIN MENU to return to the home screen or EXIT to quit completely.", Play::cWhite, true);
		DrawOptions(EndMenu); 
		currentEndGameOption = MenuEncounter(currentEndGameOption, 1, MAIN_MENU); // Handles selection between the drawn options
		TrophyGet[currentDifficulty - 1] = true; // Achieves the trophy for the current difficulty mode
		if (screenTimer < 60) {
			TrophyGet[TrophyMap.size() - 1] = true; // Achieves the speedy trophy if the game was cleared in less than a minute
		}
		else if (screenTimer >= 600) { // Achieves the slow trophy if the game was cleared in ten or more minutes
			TrophyGet[TrophyMap.size() - 2] = true;
		}
		if (!TrophyGet[4]) { // Checks for a trophy (if no enemies were cleared during the game)
			TrophyGet[4] = currentLevel->GetPacifistClear();
		} if (!TrophyGet[5]) { // Checks for a trophy (if all enemies on at least seven floors were cleared during the game)
			TrophyGet[5] = currentLevel->GetLeaderlessClear();
		}
		MainMenu[0] = " ";
		PrintInfo("[Trophy]",trophyFile,TrophyMenu, TrophyMap, TrophyGet); // Overwrites the current trophy list 
		Play::PresentDrawingBuffer(); 
		break;
	default:
		// Current game state is invalid, directly close game window
		inactiveGame = true;
	}
	return inactiveGame;

}