#ifndef PLAYER_H
#define PLAYER_H

/* Includes: 
Header files */
#include "Config.h"

/* Base class */
#include "Creature.h"

class Player : public Creature {

public: 
	Player();
	~Player();

	Play::Point2D GetPosition();

	void SetPosition(Play::Point2D userPos);
	void SetPosition(std::vector<Play::Point2D> levelExits);

	void SetColour(std::vector<Play::Colour> colourList, int newColour);
	//void SetBoundaries(std::vector<Play::Point2D> levelBounds);

	// Check whether or not the player bumps into a wall
	void CheckCollision();
	void DrawCharacter() override;

	bool HandleControls(std::vector<Play::KeyboardButton> gameControls);

private: 

	//Play::Point2D playerPos;
	Play::Point2D endPos;
	Play::Point2D playerDirection;
	Play::Colour playerColour[1] = { Play::cWhite };
	
};


#endif