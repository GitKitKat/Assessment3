#ifndef ENEMY_H
#define ENEMY_H

/* Includes: 
Base class */
#include "Creature.h"

class Enemy : public Creature {

public:
	Enemy();
	~Enemy();

	void CheckCollision() override;
	void DrawCharacter() override;

	int GetDirection();
	bool HandleControls(int randomNum);
	Play::Point2D GetPosition();

	//void SetPosition(Play::Point2D designatedPos);
	//void SetPosition(std::vector<Play::Point2D> enemyPos);

private:

	std::vector<Play::Point2D> enemyPositions;
//	Play::Point2D enemyPos;
	bool enemyCleared;
	bool enemyMovement;
	Play::Colour enemyColours[2] = { Play::cRed, Play::cGreen };

};


#endif