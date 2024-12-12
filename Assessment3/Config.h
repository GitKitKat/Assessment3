#ifndef CONFIG_H
#define CONFIG_H

/* Globals */
//
const int DISPLAY_WIDTH = 1000;
const int DISPLAY_HEIGHT = 1000;
const int DISPLAY_TILE = 50;
const int DISPLAY_SCALE = 1;

const float SPLASH_TIMELIMIT = 3.0f;
const float PLAY_TIMETAKEN = 0;

enum GameObjectType {

	TYPE_NULL = -1,
	TYPE_PLAYER,
	TYPE_COIN,
	TYPE_LASER,

};

#endif