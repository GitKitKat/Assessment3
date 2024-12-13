#ifndef PLAYER_H
#define PLAYER_H

#include "Creature.h"
#include "Play.h"
#include "Config.h"

#include <string>
#include <vector>
#include <map>

class Player : public Creature {

public: 
	Player();
	~Player();

	void SetPosition(Play::Point2D userPos);
	void SetPosition(std::vector<Play::Point2D> levelExits);

	void SetColour(std::vector<Play::Colour> colourList, int newColour);
	void SetBoundaries(std::vector<Play::Point2D> levelBounds);

	void CheckCollision();
	void DrawPlayer();

	bool HandleControls(std::vector<Play::KeyboardButton> gameControls);

private: 

	Play::Point2D playerPos;
	Play::Point2D endPos;
	Play::Point2D newPos;
	std::map<float, std::vector<Play::Point2D>> tempBoundaries;
	
	Play::Colour playerColour[1] = { Play::cWhite };
	float posIncrease;

};


#endif