#ifndef ENEMY_H
#define ENEMY_H

#include "Creature.h"
#include "Play.h"
#include "Config.h"

#include <string>
#include <vector>

class Enemy : public Creature {

public:
	Enemy();
	~Enemy();

	void DrawEnemies();
	void SetPositions(std::vector<Play::Point2D> enemyPos);

private:

	std::vector<Play::Point2D> enemyPositions;

};


#endif