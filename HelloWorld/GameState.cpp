#include "GameState.h"
#include <string>

GameState::GameState() {

	timer = 0.0f;
	stateTimer = 0.0f;
	spriteID = 0;
	score = 0;

}

GameState::~GameState() {

}

float GameState::GetTime() {

	return timer;

}

int GameState::GetSpriteID() {

	return spriteID;

}

int GameState::GetScore() {

	return score;

}

std::string GameState::CurrentScore() {

	return "Score: " + std::to_string(score);

}

void GameState::SetTime(float elapsedTime) {

	timer += elapsedTime;

}

void GameState::SetSpriteID() {

	spriteID++;

}

void GameState::SetScore(int addPoints) {

	score += addPoints;

}