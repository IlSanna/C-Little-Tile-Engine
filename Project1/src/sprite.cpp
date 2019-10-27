#include "../headers/sprite.h"

Sprite::Sprite() {
}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
		int width, int height, float posX, float posY):
	_x(posX),_y(posY) {
	_sourceRect.x = sourceX;
	_sourceRect.y = sourceY;
	_sourceRect.w = width;
	_sourceRect.h = height;

	_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (!_spriteSheet) {
		std::cout << "Error loading the image :"<< filePath << std::endl;
	}
}

Sprite::~Sprite() {
}

void Sprite::update() {
}

void Sprite::draw(Graphics & graphics, int x, int y) {
	SDL_Rect destinationRectangle = {
		 x,  y,
		_sourceRect.w * globals::SPRITE_SCALE,//this will modify the scale
		_sourceRect.h * globals::SPRITE_SCALE
	};
	graphics.blitSurface(_spriteSheet, &_sourceRect, &destinationRectangle);
}

