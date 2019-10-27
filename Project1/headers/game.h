#pragma once
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "animatedSprite.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	AnimatedSprite _player;
};

