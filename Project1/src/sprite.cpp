#include "../headers/sprite.h"

Sprite::Sprite() {
}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
		int width, int height, float posX, float posY):
	_x(posX),_y(posY) {
	_sourceRect.x = sourceX;
	_sourceRect.y = sourceY;
	_sourceRect.w = width;//fix strano qwerty -1
	_sourceRect.h = height;//fix strano qwerty -1

	//create the sprite image
	_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (!_spriteSheet) {
		std::cout << "Error loading the image :"<< filePath << std::endl;
	}
	//set BB
	_boundingBox = Rectangle(_x, _y, _sourceRect.w * globals::SPRITE_SCALE, _sourceRect.h * globals::SPRITE_SCALE);
}

Sprite::~Sprite() {
}

void Sprite::update() {
	////update BB position
	//_boundingBox = Rectangle(//magic numbers to make a better BB
	//	_x,// +3,
	//	_y,// +1, 
	//	(_sourceRect.w -1) * globals::SPRITE_SCALE,//-3
	//	(_sourceRect.h -2)* globals::SPRITE_SCALE
	//);
}

void Sprite::draw(Graphics & graphics, int x, int y) {
	SDL_Rect destinationRectangle = {
		 x,  y,
		_sourceRect.w * globals::SPRITE_SCALE,//this will modify the scale
		_sourceRect.h * globals::SPRITE_SCALE
	};
	graphics.blitSurface(_spriteSheet, &_sourceRect, &destinationRectangle);
}

const Rectangle Sprite::getBoundingBox() const{
	return _boundingBox;
}
//determine on which side we are colliding
const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	//determine how much of the other rectangle is inside our BB
	int amountRight, amountLeft, amountTop, amountBottom;
	//the lowest amount is the side we are colliding with
	amountRight = getBoundingBox().getRight() - other.getLeft();
	amountLeft = other.getRight() - getBoundingBox().getLeft();
	amountTop = other.getBottom() - getBoundingBox().getTop();
	amountBottom = getBoundingBox().getBottom() - other.getTop();
	//create an array of all this values
	int values[4] = { abs(amountRight),abs(amountLeft),abs(amountTop),abs(amountBottom) };
	
	int min = values[0];
	for (int i = 0; i < 4; i++) {
		if(values[i]<min){
			min = values[i];
		}
	}

	return  min == abs(amountRight) ? sides::RIGHT :
			min == abs(amountLeft) ? sides::LEFT :
			min == abs(amountTop) ? sides::TOP :
			min == abs(amountBottom) ? sides::BOTTOM :
			sides::NONE;
}

