#include "../headers/level.h"

Level::Level() {
}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0,0)) {
	loadMap(_mapName, graphics);
}

void Level::update(float elapsedTime) {
}

void Level::draw(Graphics &graphics) {
	SDL_Rect sourceRect = {
		0,0,64,64
	};
	SDL_Rect destinationRectangle;
	for (int x = 0; x < _size.x / 64; x++) { //divide the map width in tile
		for (int y = 0; y < _size.y / 64; y++) {
			destinationRectangle.x = x * 64 * globals::SPRITE_SCALE;//* globals::SPRITE_SCALE
			destinationRectangle.y = y * 64 * globals::SPRITE_SCALE;
			destinationRectangle.w = 64 * globals::SPRITE_SCALE;
			destinationRectangle.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(_backgroundTexture, &sourceRect, &destinationRectangle);
		}
	}
}

void Level::loadMap(std::string mapName, Graphics & graphics) {
	_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage("content/sprite/bkBlue.png"));
	_size = Vector2(globals::SCREEN_WIDTH * 2, globals::SCREEN_HEIGHT * 2);
}
