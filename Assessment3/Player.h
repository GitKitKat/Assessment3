#ifndef PLAYER_H
#define PLAYER_H

/* Includes: 
Base class */
#include "Creature.h"

class Player : public Creature {
public: 
	/* Declarations: */
	// Constructor
	Player();
	// Destructor
	~Player();

	// Updates the player with the starting position and position of the exit
	void SetExits(std::vector<Play::Point2D> levelExits);
	// Updates the player's colour
	void SetColour(std::vector<Play::Colour> colourList, int newColour);
	// Handles the player's movement through user input
	bool HandleControls(std::vector<Play::KeyboardButton> gameControls);
	// Draws the player as a 2D rectangle in the game window
	void DrawCharacter();

private: 
	// Stores the location of the level's exit
	Play::Point2D endPos;
	// Stores the direction the player last moved in 
	Play::Point2D playerDirection;
	// Stores the player's current colour
	Play::Colour playerColour[1] = { Play::cBlue };
	
};


#endif