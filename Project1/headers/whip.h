#pragma once
#include "graphics.h"
#include "animatedSprite.h"

class Whip : public AnimatedSprite {
public:
	Whip();
	Whip(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime, float x, float y,Direction facing);

	virtual void setupAnimation();
	virtual void animationDone(std::string currentAnimation);
	
	void handleTileCollision(std::vector<Rectangle> &others);

	void setX(float value);
	void setY(float value);
private:
	float _dx = 4;
	float _dy;//deltaX and deltaY, the change in x and y direction
	Direction _facing;
	Vector2 _lastPos;
};
