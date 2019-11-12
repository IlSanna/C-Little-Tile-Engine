#pragma once
#include "rectangle.h"
#include "globals.h"
#include <string>

class Door : public Rectangle {
public:
	Door(Rectangle r, std::string destination) :
		Rectangle(r.getLeft() ,//* globals::SPRITE_SCALE, 
				  r.getTop(),//* globals::SPRITE_SCALE, 
				  r.getRight(),//* globals::SPRITE_SCALE, 
				  r.getHeight()),//* globals::SPRITE_SCALE),
		_destination(destination) 
	{}

	const Rectangle getRectangle() { return getRect(); }
	const std::string getDestination() const { return _destination; }

private:
	std::string _destination;
};