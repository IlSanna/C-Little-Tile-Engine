#pragma once
#include "graphics.h"


class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void update(float elapsedTime);
	void draw(Graphics &graphics);
private:
	SDL_Texture* _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;//position on the tileset
	Vector2 _position;//position on the map
};