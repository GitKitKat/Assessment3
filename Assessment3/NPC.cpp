#include "NPC.h"

#include "Play.h"

NPC::NPC() {

	itemsFile = "Data\\Textfiles\\ItemList.txt";
	levelComplete = false;

}

NPC::~NPC() {



}

void NPC::SetDialogue() {

	dialogueIndex++;

}

std::vector<std::string> NPC::GetDialogueDesc() {

	return dialogueDesc;

}

std::vector<std::string> NPC::GetDialogueOptions() {

	return dialogueChoices;

}

void NPC::SetDialogue(int currentLevel) {

	if (levelComplete) {
		dialogueIndex = 10;
	}
	else {
		dialogueIndex = 0;
	} 
	LoadDialogue(currentLevel);

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

			if (newLine.rfind(newLevel, 0) == 0) {

				dialogueFound = true;

			}
			if (newLine.rfind(newDialogue, 0) == 0) {

				dialogueFound = true;

			}

			if (levelFound && dialogueFound) {

				if (newLine.rfind("(NPC)", 0) == 0) {

					dialogueDesc.push_back(newLine.substr(6));

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

void NPC::PrintDialogue() {



}