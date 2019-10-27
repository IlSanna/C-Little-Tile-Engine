#pragma once


namespace globals {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const float SPRITE_SCALE = 2.0f;
}

struct Vector2{

	int _x, _y;

	Vector2() :_x(0), _y(0) {}
	Vector2(int x, int y) :_x(x), _y(y) {}

	Vector2 zero() { return Vector2(0, 0); }
};