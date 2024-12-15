#include "NPC.h"

#include "Play.h"

NPC::NPC() {

	itemsFile = "Data\\Textfiles\\Dialogues.txt";
	levelComplete = false;
	earlyExit = false;

}

NPC::~NPC() {



}

void NPC::SetDialogue(int increase) {

	dialogueIndex += increase;

}

std::vector<std::string> NPC::GetDialogueDesc() {

	return dialogueDesc;

}

std::vector<std::string> NPC::GetDialogueOptions() {

	return dialogueChoices;

}

void NPC::ResetDialogue(int currentLevel, int increase) {

	if (earlyExit == false && increase >= 0) {
		SetDialogue(increase);
	}
	else if (increase == -1) {
		dialogueIndex = 0;
		earlyExit = false;
		levelComplete = false;
	}
	LoadDialogue(currentLevel);

}

bool NPC::DialogueEnd() {
	return levelComplete;
}

bool NPC::NextDialogue() {
	return earlyExit;
}

bool NPC::CheckCollision(Play::Point2D playerPos) {

	if (characterPos == playerPos) {
		return true;
	}
	else {
		return false;
	}

}

void NPC::DrawCharacter() {

	Play::DrawRect(characterPos, { characterPos.x + float(DISPLAY_TILE), characterPos.y + float(DISPLAY_TILE) }, Play::cBlue, true);

}

void NPC::LoadDialogue(int currentLevel) {

	dialogueChoices.clear();
	dialogueDesc.clear();
	std::string newDialogue = "(ID)[" + std::to_string(dialogueIndex) + "]";
	std::string newLevel = "(Level)[" + std::to_string(currentLevel) + "]";
	std::ifstream inFile(itemsFile);
	std::string newLine;
	bool dialogueFound = false;
	bool levelFound = false;

	if (!inFile.is_open()) {

		return;

	}
	else {

		// This loop reads and relays a specified interaction.
		while (std::getline(inFile, newLine)) {

			if (newLine == newLevel) {

				levelFound = true;

			}
			if (levelFound == true && newLine == newDialogue) {

				dialogueFound = true;

			}

			if (levelFound && dialogueFound) {

				if (newLine.rfind("(Name)", 0) == 0) {

					dialogueDesc.push_back(newLine.substr(6));

				}
				else if (newLine.rfind("(Exit)", 0) == 0) {

					earlyExit = true;

				}
				else if (newLine.rfind("(Final)", 0) == 0) {

					earlyExit = true;
					levelComplete = true;

				}
				else if (newLine.rfind("(Choice)", 0) == 0) {

					dialogueChoices.push_back(newLine.substr(8));

				}
				else if (newLine.rfind("(Desc)", 0) == 0) {

					dialogueDesc.push_back(newLine.substr(6));

				}

				else if (newLine == "[End]") {

					dialogueFound = false;
					levelFound = false;
					break;

				}

			}

		}

	}

	inFile.close();

}

int NPC::PrintDialogue(bool currentControls) {

	Play::DrawRect({ DISPLAY_TILE, DISPLAY_HEIGHT * 0.5f }, { DISPLAY_WIDTH - DISPLAY_TILE, DISPLAY_HEIGHT - DISPLAY_TILE }, Play::cWhite, true);
	Play::DrawRect({ DISPLAY_TILE + DISPLAY_TILE * 0.25f, DISPLAY_HEIGHT * 0.5f + DISPLAY_TILE * 0.25f }, { DISPLAY_WIDTH - (DISPLAY_TILE + DISPLAY_TILE * 0.25f), DISPLAY_HEIGHT - (DISPLAY_TILE + DISPLAY_TILE * 0.25f) }, Play::cBlack, true);
	int a = int(dialogueDesc.size());

	for (int i = 0; i < a; i++) {

		const char* charPtr = dialogueDesc[i].c_str();
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - ((3 + i) * DISPLAY_TILE) }, charPtr, Play::cWhite, true);

	}
	if (dialogueChoices.size() == 0) {
		if (earlyExit == false) {
			Play::DrawDebugText({ 6 * DISPLAY_TILE, DISPLAY_HEIGHT - (5 + a) * DISPLAY_TILE }, "Press SPACE to continue", Play::cWhite, true);
		}
	}
	Play::DrawDebugText({ DISPLAY_WIDTH - (6 * DISPLAY_TILE), DISPLAY_HEIGHT - (5 + a) * DISPLAY_TILE }, "Press X to leave", Play::cWhite, true);
	std::vector<std::string> ctrlArr;

	if (currentControls == true) {
		ctrlArr = { "A", "W", "D", "S" };
	}
	else {
		ctrlArr = { "LEFT", "UP", "RIGHT", "DOWN" };
	}
	for (int i = 0; i < dialogueChoices.size(); i++) {

		const char* choicePtr = dialogueChoices[i].c_str();
		const char* controlsPtr = ctrlArr[i].c_str();
		Play::DrawDebugText({ DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT - (7 + i) * DISPLAY_TILE }, choicePtr, Play::cWhite, true);
		Play::DrawDebugText({ (DISPLAY_WIDTH * 0.5f) - DISPLAY_TILE * 2, DISPLAY_HEIGHT - (7 + i) * DISPLAY_TILE }, controlsPtr, Play::cWhite, true);

	}
	return dialogueChoices.size();
}