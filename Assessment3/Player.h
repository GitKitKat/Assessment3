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

	//void SetPosition(int xPos, int yPos);

	//void HandleControls();

private: 

	int playerXpos;
	int playerYpos;
	

	Play::CreateGameObject(TYPE_PLAYER, { 115,600 }, 50, 'player1');

};


#endif