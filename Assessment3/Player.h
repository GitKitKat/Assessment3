#ifndef PLAYER_H
#define PLAYER_H

#include "Creature.h"
#include "Play.h"
#include "Config.h"

#include <string>

class Player : public Creature {

public: 
	Player();
	~Player();

	void SetPosition(Play::Point2D userPos);
	void SetColour(Play::Colour arr[], int newColour);

	void DrawPlayer();

	//void HandleControls();

private: 

	Play::Point2D playerPos;
	Play::Colour playerColour[1] = { Play::cWhite };

};


#endif