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

	//virtual void SetColour(int newColour);

	void SetPosition(Play::Point2D designatedPos);
	void SetBoundaries(std::vector<Play::Point2D> levelBounds);
	virtual void DrawCharacter() = 0;

protected:

	float posIncrease;
	Play::Point2D newPos;
	Play::Point2D characterPos;

	std::map<float, std::vector<Play::Point2D>> tempBoundaries;

};

#endif