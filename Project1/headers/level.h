#pragma once
#include "graphics.h"

class Level {
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics);
	void update(float elapsedTime);
	void draw(Graphics &graphics);
private:
	std::string _mapName;
	Vector2 _spawnPoint;

	Vector2 _size;//width and height of the entire map
	SDL_Texture* _backgroundTexture;

	void loadMap(std::string mapName, Graphics &graphics);
};