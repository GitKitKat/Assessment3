#ifndef ENEMY_H
#define ENEMY_H

/* Includes: 
Base class */
#include "Creature.h"

class Enemy : public Creature {

public:
	Enemy();
	~Enemy();

	void CheckCollision();
	void DrawCharacter() override;

	bool GetState();
	void SetState();

	std::vector<std::string> GetInteraction();
	int InteractionChoice();
	std::vector<std::string> InteractionLimit();

	void SetInteraction(int difficulty);
	void LoadInteraction(int difficulty, std::string interactionIndex);
	int GetRandom(int numLimit);
	bool HandleControls(int randomNum);
	Play::Point2D GetPosition();

private:

	std::vector<std::string> interactionLimit;
	std::vector<std::string> enemyInteraction;
	bool enemyCleared;
	bool enemyMovement;
	Play::Colour enemyColours[2] = { Play::cRed, Play::cGreen };
	std::string itemsFile;

};


#endif