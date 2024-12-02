#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Config.h"
#include "Play.h"

class GameState;

class GameManager {
public:
	void Initialise();
	void Deinitialise();
	bool Update(float elapsedTime);
	static GameManager* GetGameManager();

private:
	GameManager();
	~GameManager();
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	static GameState* currentGameState;
};

#endif