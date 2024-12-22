/* Includes: 
This file's header */
#include "Creature.h"

Creature::Creature() {
	/* Definitions: */
	// 
	creaturePos = { 0.0f, 0.0f };
	newPos = { 0.0f, 0.0f };

}

Creature::~Creature() {
	// Destructor
}

Play::Point2D Creature::GetPosition() {
	return creaturePos;
}

void Creature::SetPosition(Play::Point2D designatedPos) {
	creaturePos = designatedPos;
}

void Creature::SetBoundaries(std::map<float, std::vector<Play::Point2D>> levelBounds) {
	openBoundaries = levelBounds;
}

void Creature::CheckCollision() {
	// Determines whether or not the creature's next move will hit a wall
	bool colliderCheck = true;
	// Used to determine which horizontal line the level starts on
	auto itr = openBoundaries.begin(); 

	for (float i = itr->first; i < float(openBoundaries.size() + itr->first); i++) {
		// Checks which horizontal line the creature's next vertical move is in 
		// e.g. (with x representing a tile) the player starting at (0, 0) would go up a tile to horizontal (0, 1x) 
		if (newPos.y <= (DISPLAY_TILE * i) && newPos.y > (DISPLAY_TILE * i) - DISPLAY_TILE) {
			
			for (Play::Point2D j : openBoundaries[i]) {
				// Checks whether the creature's next horizontal move is in a passable tile 
				if (newPos.x >= j.x && newPos.x < j.x + DISPLAY_TILE) {

					colliderCheck = false;

				}

			}

		}

	}
	// Updates the creature's position if the move hit no walls
	if (!colliderCheck) {
		SetPosition(newPos);
	}

}