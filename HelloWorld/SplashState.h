#ifndef SPLASHSTATE_H
#define SPLASHSTATE_H

#include "GameState.h"

class SplashState : public GameState {

public:
	SplashState();

	virtual void Update(float elapsedTime) override;
	virtual void Draw() override;
	virtual GameState* CheckTransitions() override;

private:
	MainMenuScreen currentlySelectedOption;

};

#endif