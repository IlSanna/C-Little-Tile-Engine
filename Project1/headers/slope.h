#pragma once
#include "globals.h"
#include <cmath>
#include "rectangle.h"

class Slope {
public:
	Slope();
	Slope(Vector2 p1, Vector2 p2) :
		_p1(p1),_p2(p2)
	{
		if (_p1.x - _p2.x != 0) {
			//calculate the gradient of a line
			_slope = (fabs(_p2.y) - fabs(_p1.y)) /
					 (fabs(_p2.x) - fabs(_p1.x));
		}
	}
	const float getSlope() const {
		return _slope;
	}

	//check for collisions with the player, return true if all the 16 cases are true
	const bool collideWith(const Rectangle &other) const {
		return (other.getRight() >= _p2.x &&//case right
				other.getLeft()  <= _p1.x &&
				other.getTop()   <= _p2.y &&
				other.getBottom() >= _p1.y) ||

			   (other.getRight() >= _p1.x &&//case right
				other.getLeft()  <= _p2.x &&
				other.getTop()   <= _p1.y &&
				other.getBottom() >= _p2.y) ||

			   (other.getRight() >= _p2.x &&//case left
				other.getLeft()  <= _p1.x &&
				other.getTop()   <= _p1.y &&
				other.getBottom() >= _p2.y) ||

			   (other.getRight() >= _p1.x &&//case left
				other.getLeft()  <= _p2.x &&
				other.getTop()   <= _p2.y &&
				other.getBottom() >= _p1.y);
		
	}

	const Vector2 getP1() const { return _p1; }
	const Vector2 getP2() const { return _p2; }
private:
	Vector2 _p1, _p2;
	float _slope;
};