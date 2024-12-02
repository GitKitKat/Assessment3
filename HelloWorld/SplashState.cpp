/* Includes: Standard header file */
/* Includes: This file's header */
#include "SplashState.h"

// Other includes
#include "Config.h"
//#include "MainMenuState.h"
#include "Play.h"

SplashState::SplashState() {

	currentlySelectedOption = START;

}

void SplashState::Update(float elapsedTime) {

	stateTimer += elapsedTime;

	if (currentGamescreen == MAIN_MENU) {

		switch (currentlySelectedOption) {
		case START:
		{
			// Check for transitions
			if (Play::KeyPressed(Play::KEY_S))
			{
				currentlySelectedOption = OPTIONS;
			}

			break;
		}
		case OPTIONS:
		{
			if (Play::KeyPressed(Play::KEY_W))
			{
				currentlySelectedOption = START;
			}
			else if (Play::KeyPressed(Play::KEY_S))
			{
				currentlySelectedOption = QUIT;
			}

			break;
		}
		case QUIT:
		{
			// Check for transitions
			if (Play::KeyPressed(Play::KEY_W))
			{
				currentlySelectedOption = OPTIONS;
			}

			break;
		}
		}

	}

}

void SplashState::Draw() {
	
	switch (currentGamescreen) {
	case SPLASH: {

		Play::ClearDrawingBuffer(Play::cOrange);
		Play::DrawFontText("72px", "SPLASH", { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, Play::CENTRE);
		break;
	}
	case MAIN_MENU: {

		Play::ClearDrawingBuffer(Play::cBlack);
		Play::DrawFontText("32px", "START", { DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) + 50.0f }, Play::LEFT);
		Play::DrawFontText("32px", "OPTIONS", { DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) }, Play::LEFT);
		Play::DrawFontText("32px", "QUIT", { DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) - 50.0f }, Play::LEFT);

		switch (currentlySelectedOption) {
		case START: {
			// Draw arrow next to "Start"
			Play::DrawFontText("32px", "---->", { (DISPLAY_WIDTH / 2) - 50.0f, (DISPLAY_HEIGHT / 2) + 50.0f }, Play::CENTRE);
			break;
		}
		case OPTIONS: {
			// Draw arrow next to "Options"
			Play::DrawFontText("32px", "---->", { (DISPLAY_WIDTH / 2) - 50.0f, (DISPLAY_HEIGHT / 2) }, Play::CENTRE);
			break;
		}
		case QUIT: {
			// Draw arrow next to "Quit"
			Play::DrawFontText("32px", "---->", { (DISPLAY_WIDTH / 2) - 50.0f, (DISPLAY_HEIGHT / 2) - 50.0f }, Play::CENTRE);
			break;
		}
		}
		break;
	}
	case PLAY: {

		// Tile creation
		for (int i = 1; i < DISPLAY_TILE; i++) {

			Play::DrawLine({ (DISPLAY_WIDTH / DISPLAY_TILE) * i,0 }, { (DISPLAY_WIDTH / DISPLAY_TILE) * i,DISPLAY_HEIGHT }, Play::cGrey);
			Play::DrawLine({ 0,(DISPLAY_HEIGHT / DISPLAY_TILE) * i }, { DISPLAY_WIDTH,(DISPLAY_HEIGHT / DISPLAY_TILE) * i }, Play::cGrey);

		}

	}
	}

	Play::PresentDrawingBuffer();

}

GameState* SplashState::CheckTransitions() {

	if (currentGamescreen == START) {

		if (stateTimer >= SPLASH_TIMELIMIT)
		{
			currentGamescreen = MAIN_MENU;
		}
	}
	else if (currentGamescreen == MAIN_MENU) {

		if (Play::KeyPressed(Play::KEY_SPACE))
		{
			if (currentlySelectedOption == START)
			{
				currentGamescreen = PLAY;
				//return new PlayState();
			}
			else if (currentlySelectedOption == QUIT)
			{
				//return new QuitState();
			}
		}
	}

	return nullptr;

}