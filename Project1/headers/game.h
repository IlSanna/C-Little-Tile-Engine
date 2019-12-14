#pragma once
#include "graphics.h"
#include "input.h"
#include "player.h"
#include "level.h"
#include "AssetManager.h"
#include "AudioManager.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();

	void handleInput(Input &input, bool &retflag);
	void draw(Graphics &graphics);
	void update(float elapsedTime, Input& input);
	void checkCollisions(float elapsedTime);

	Player _player;
	Level _level;
	Graphics _graphics;
	AssetManager* _assetManager;
	AudioManager* _audioManager;

	bool retflag;
};

