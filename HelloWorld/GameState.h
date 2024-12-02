#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include "Config.h"

class GameState {
public: 

	GameState();
	~GameState();

	float GetTime();
	int GetSpriteID();
	int GetScore();
	std::string CurrentScore();
	std::string CurrentScreen();

	void SetTime(float elapsedTime);
	void SetSpriteID();
	void SetScore(int addPoints);

	virtual void Update(float elapsedTime) = 0;
	virtual void Draw() = 0;
	virtual GameState* CheckTransitions() = 0;

protected:
	float timer;
	float stateTimer;
	int score;
	int spriteID;

	GameScreen currentGamescreen = SPLASH;
	MainMenuScreen currentMenuOption = START;

};

#endif