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

void Creature::SetBoundaries(std::map<float, std::vector<Play::Point2D>> levelBounds) {

	tempBoundaries = levelBounds;

}