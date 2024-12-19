#ifndef PLAYER_H
#define PLAYER_H

/* Includes: 
Header files */
#include "Config.h"

/* Base class */
#include "Creature.h"

class Player : public Creature {

public: 
	/* Declarations: */
	// Constructor
	Player();
	// Destructor
	~Player();


	void SetExits(std::vector<Play::Point2D> levelExits);

	void SetColour(std::vector<Play::Colour> colourList, int newColour);
	// Draws the NPC as a 2D rectangle in the game window
	void DrawCharacter();

	bool HandleControls(std::vector<Play::KeyboardButton> gameControls);

private: 

	//Play::Point2D playerPos;
	Play::Point2D endPos;
	Play::Point2D playerDirection;
	Play::Colour playerColour[1] = { Play::cBlue };
	
};


#endif