#pragma once
#include "../headers/game.h"
#include "../headers/graphics.h"
#include "../headers/input.h"
#include "../headers/sprite.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	Sprite _player;
};

