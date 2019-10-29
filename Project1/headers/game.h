#pragma once
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "player.h"
#include "level.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	Player _player;
	Level _level;
};

