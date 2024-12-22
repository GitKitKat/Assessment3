/* Includes:
This file's header */
#include "NPC.h"

NPC::NPC() {
	/* Definitions: */
	// 
	dialogueIndex = 0;
	itemsFile = "Data\\Textfiles\\Dialogues.txt";
	earlyExit = false;
	levelComplete = false;

}

NPC::~NPC() {
	// Destructor
}

bool NPC::CheckCollision(Play::Point2D playerPos) {

	if (creaturePos == playerPos) { // Checks whether the player has encountered the NPC 
		return true;
	}
	else {
		return false;
	}

}

std::vector<std::string> NPC::GetDialogueDesc() {
	return dialogueDesc;
}

std::vector<std::string> NPC::GetDialogueOptions() {
	return dialogueChoices;
}

void NPC::SetDialogue(int increase) {
	dialogueIndex += increase;
}

void NPC::ResetDialogue(int currentLevel, int increase) {

	if (!earlyExit && increase >= 0) { // Checks whether the conversation has ended
		if (dialogueIndex < 100) { // Continues the dialogue normally
			SetDialogue(increase);
		}
		else {
			if (increase == 1) { // Reverts the dialogue to the conversation ender
				SetDialogue(-99);
			}
			else { // Continues the dialogue normally
				SetDialogue(-100);
			}
		}
	}
	else if (increase == -1) { // Resets the dialogue to the beginning
		dialogueIndex = 0;
		earlyExit = false;
		levelComplete = false;
	}
	else if (earlyExit && increase == 1) { // Displays a dialogue with options to continue normally or end the conversation
		earlyExit = false;
		SetDialogue(100);
	}
	LoadDialogue(currentLevel);

}

bool NPC::DialogueEnd() {
	return levelComplete;
}

bool NPC::NextDialogue() {
	return earlyExit;
}

void NPC::DrawCharacter() {
	Play::DrawRect(creaturePos, { creaturePos.x + float(DISPLAY_TILE), creaturePos.y + float(DISPLAY_TILE) }, Play::cBlue, true);
}

void NPC::LoadDialogue(int currentLevel) {

	dialogueChoices.clear();
	dialogueDesc.clear(); // Clears the previously loaded dialogue
	// Stores the ID of the dialogue to be loaded
	std::string newDialogue = "(ID)[" + std::to_string(dialogueIndex) + "]";
	std::string newLevel = "(Level)[" + std::to_string(currentLevel) + "]";
	// Opens the text file for reading
	std::ifstream inFile(itemsFile);
	// Stores each line of the opened file as a new string
	std::string newLine;
	// Whether the specified dialogue was found
	bool dialogueFound = false;
	// Whether the current level (ID) was found
	bool levelFound = false;

	if (!inFile.is_open()) { // Returns if the file could not be opened
		return;
	}
	else {
		// This loop reads and relays a specified dialogue
		while (std::getline(inFile, newLine)) {

			if (newLine == newLevel) { // Checks if dialogues with the current level ID were found
				levelFound = true;
			}

			if (levelFound && newLine == newDialogue) { // Checks if the specified level's dialogue was found
				dialogueFound = true;
			}

			if (levelFound && dialogueFound) {

				if (newLine.rfind("(Name)", 0) == 0) { // Stores the name of the currently speaking NPC
					dialogueDesc.push_back(newLine.substr(6));
				}
				else if (newLine.rfind("(Exit)", 0) == 0) { // Checks if the current dialogue is the end of a conversation
					earlyExit = true;
				}
				else if (newLine.rfind("(Final)", 0) == 0) { // Checks if the current dialogue is the end of a cutscene
					levelComplete = true;
				}
				else if (newLine.rfind("(Desc)", 0) == 0) { // Stores each line of the dialogue 
					dialogueDesc.push_back(newLine.substr(6));
				}
				else if (newLine.rfind("(Choice)", 0) == 0) { // Stores the available choices (player responses) to the current dialogue
					dialogueChoices.push_back(newLine.substr(8));
				}
				else if (newLine == "[End]") { // Ends the reading for the file

					dialogueFound = false;
					levelFound = false;
					break;

				}

			}

		}

	}
	inFile.close(); // Closes the file

}

int NPC::PrintDialogue(bool currentControls) {
	// Draws a black rectangle with a white border 
	Play::DrawRect({ DISPLAY_TILE, DISPLAY_HEIGHT * 0.5f }, { DISPLAY_WIDTH - DISPLAY_TILE, DISPLAY_HEIGHT - DISPLAY_TILE }, Play::cWhite, true);
	Play::DrawRect({ DISPLAY_TILE + DISPLAY_TILE * 0.25f, DISPLAY_HEIGHT * 0.5f + DISPLAY_TILE * 0.25f }, { DISPLAY_WIDTH - (DISPLAY_TILE + DISPLAY_TILE * 0.25f), DISPLAY_HEIGHT - (DISPLAY_TILE + DISPLAY_TILE * 0.25f) }, Play::cBlack, true);

	for (int i = 0; i < dialogueDesc.size(); i++) {
		if (i == 0) { // Draws the NPC's name
			Play::DrawFontText("32px", dialogueDesc[i], { DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - ((3 + i) * DISPLAY_TILE) }, Play::CENTRE);
		}
		else { // Draws the dialogue line by line
			Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - ((3 + i) * DISPLAY_TILE) }, dialogueDesc[i].c_str(), Play::cWhite, true);
		}
	}
	if (dialogueChoices.size() == 0) { // If there are no available options, draws default options (CONTINUE, QUIT)
		if (!earlyExit) { 
			Play::DrawDebugText({ 6 * DISPLAY_TILE, DISPLAY_HEIGHT - (5 + int(dialogueDesc.size())) * DISPLAY_TILE }, "Press SPACE to continue", Play::cCyan, true);
		}
		else {
			Play::DrawDebugText({ 6 * DISPLAY_TILE, DISPLAY_HEIGHT - (5 + int(dialogueDesc.size())) * DISPLAY_TILE }, "Press ESC to quit", Play::cCyan, true);
		}
	} 
	// Draws an option to leave (exits the dialogue but not the level) if the conversation ends. The player can continue the conversation by interacting with the NPC again
	Play::DrawDebugText({ DISPLAY_WIDTH - (6 * DISPLAY_TILE), DISPLAY_HEIGHT - (5 + int(dialogueDesc.size())) * DISPLAY_TILE }, "Press X to leave", Play::cCyan, true);
	// Stores the currently active controls as separate strings
	std::vector<std::string> ctrlArr;
	if (currentControls) {  // If true, current inputs are W, A, S, D keys
		ctrlArr = { "A", "W", "D", "S" };
	}
	else {
		ctrlArr = { "LEFT", "UP", "RIGHT", "DOWN" };
	}
	for (int i = 0; i < dialogueChoices.size(); i++) { // Draws the available choices in an encounter and their corresponding input key

		Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (7 + i) * DISPLAY_TILE }, dialogueChoices[i].c_str(), Play::cCyan, true);
		Play::DrawDebugText({ (DISPLAY_WIDTH * 0.5f) - DISPLAY_TILE * 2, DISPLAY_HEIGHT - (7 + i) * DISPLAY_TILE }, ctrlArr[i].c_str(), Play::cCyan, true);

	}
	return dialogueChoices.size();

}