/* Includes: 
This file's header */
#include "Creature.h"

Creature::Creature() {
	/* Definitions: */
	// 
	characterPos = { 0.0f, 0.0f };
	newPos = { 0.0f, 0.0f };

}

Creature::~Creature() {
	// Destructor
}

Play::Point2D Creature::GetPosition() {

	return characterPos;

}

void Creature::SetPosition(Play::Point2D designatedPos) {

	characterPos = designatedPos;

}

void Creature::SetBoundaries(std::map<float, std::vector<Play::Point2D>> levelBounds) {

	tempBoundaries = levelBounds;

}

void Creature::CheckCollision() {

	bool colliderCheck = true;
	auto itr = tempBoundaries.begin();
	float z;
	if (itr->first == 0) {

		z = float(tempBoundaries.size());

	}
	else {

		z = float(tempBoundaries.size() + itr->first);

	}

	for (float i = itr->first; i < z; i++) {

		if (newPos.y <= (DISPLAY_TILE * i) && newPos.y > (DISPLAY_TILE * i) - DISPLAY_TILE) {

			for (Play::Point2D j : tempBoundaries[i]) {

				if (newPos.x >= j.x && newPos.x < j.x + DISPLAY_TILE) {

					colliderCheck = false;

				}

			}

		}

	}

	if (!colliderCheck) {

		SetPosition(newPos);

	}

}