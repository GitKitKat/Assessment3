/* Includes: 
This file's header */
#include "Creature.h"

Creature::Creature() {

	characterPos = { 0.0f, 0.0f };
	newPos = { 0.0f, 0.0f };

}

Creature::~Creature() {



}

void Creature::SetPosition(Play::Point2D designatedPos) {

	characterPos = designatedPos;

}

void Creature::SetBoundaries(std::vector<Play::Point2D> levelBounds) {

	std::vector<Play::Point2D> tempVector;
	tempBoundaries.clear();

	for (int i = 0; i < levelBounds.size(); i++) {

		for (int j = 0; j < levelBounds.size(); j++) {

			if (levelBounds[j].y <= (DISPLAY_TILE * i) && levelBounds[j].y > (DISPLAY_TILE * i) - DISPLAY_TILE) {

				tempVector.push_back(levelBounds[j]);

			}

		}

		if (tempVector.size() > 0) {

			tempBoundaries.insert({ i, tempVector });

		}

		tempVector.clear();

	}
}