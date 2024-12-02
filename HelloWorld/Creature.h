#ifndef CREATURE_H
#define CREATURE_H

// object is the instance
//#include "Play.h"
// parent class for player, npcs and enemies

class Creature {
public:

	Creature();
	~Creature();

	//virtual void SetInitialStats();
	
private:
	//GameObject& CreatureInfo;
	int health;
	int physAtk;
	int magiAtk;

};

#endif