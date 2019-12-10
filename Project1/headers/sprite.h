#pragma once 
#include <SDL.h>
#include "graphics.h"
#include "rectangle.h"
#include <vector>

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
			int width, int height, float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	void draw(Graphics &graphics, int x, int y);

	Rectangle getBoundingBox() ;
	const sides::Side getCollisionSide(Rectangle &other) const;
protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	Rectangle _boundingBox;
	float _x, _y;
};




