#pragma once
#include "globals.h"

class Rectangle {
public:
	Rectangle(){}
	Rectangle(int x, int y, int width, int height, bool left = false) :
		_x(x), _y(y), _width(width), _height(height),_left(left) {

	}
	const int getCenterX() const { return _x + _width / 2; }
	const int getCenterY() const { return _y + _height / 2; }
	
	const int getLeft() const { return _x; }
	const int getRight() const { return _x + _width; }
	const int getTop() const { return _y; }
	const int getBottom() const { return _y + _height; }

	const int getWidth() const { return _width; }
	const int getHeight() const { return _height; }

	//helper function to get the side
	const int getSide(sides::Side side) const {
		return  side == sides::TOP ? getTop() :
				side == sides::BOTTOM ? getBottom() :
				side == sides::LEFT ? getLeft() :
				side == sides::RIGHT ? getRight() :
				sides::NONE;
	}
	//AABB test
	const bool collideWith(const Rectangle &other) const{
		return  getRight() >= other.getLeft() &&
				getBottom() >= other.getTop() &&
				other.getRight() >= getLeft() &&
				other.getBottom() >= getTop();
	}
	//check if is a valid rectangle
	const bool isValidRectangle() const {
		return _x >= 0 && _y >= 0 && _width >= 0 && _height >= 0;
	}
	const Rectangle getRect() { return *this; }
	const bool isLeftSlope() { return _left; }
private:
	int _x, _y, _width, _height;
	bool _left;
};