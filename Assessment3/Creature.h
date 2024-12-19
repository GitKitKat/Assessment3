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

	// Returns the creature's current position
	Play::Point2D GetPosition();
	// Updates the creature's position (x,y) with the passed values
	void SetPosition(Play::Point2D designatedPos);
	// Passes the tiles that can be walked over (non-walls) to the creature
	void SetBoundaries(std::map<float, std::vector<Play::Point2D>> levelBounds);
	// Check whether or not the creature bumps into a wall
	void CheckCollision();

protected:

	// A float value that modifies the distance a creature can move (default = 1)
	float posIncrease;
	// Stores the creature's next position
	Play::Point2D newPos;
	// Stores the creature's current position
	Play::Point2D characterPos;
	// Stores the positions of tiles that can be walked over (non-walls)
	std::map<float, std::vector<Play::Point2D>> tempBoundaries;

};

#endif