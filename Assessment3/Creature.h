#ifndef CREATURE_H
#define CREATURE_H

/* Includes: 
Standard header files */
#include <string>
#include <vector>
#include <map>

/* Header files */
#include "Play.h"
#include "Config.h"

class Creature {

public:
	/* Declarations: */
	// Constructor
	Creature();
	// Destructor
	~Creature();

	void SetPosition(Play::Point2D designatedPos);
	void SetBoundaries(std::map<float, std::vector<Play::Point2D>> levelBounds);
	// Check whether or not the creature bumps into a wall
	void CheckCollision();

protected:

	float posIncrease;
	Play::Point2D newPos;
	Play::Point2D characterPos;

	std::map<float, std::vector<Play::Point2D>> tempBoundaries;

};

#endif