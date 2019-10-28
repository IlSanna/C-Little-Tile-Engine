#pragma once
#include "graphics.h"
#include "animatedSprite.h"

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	void moveLeft();
	void moveRight();
	void stopMoving();

	virtual void setupAnimation();
	virtual void animationDone(std::string currentAnimation);
private:
	float _dx, dy;//deltaX and deltaY, the change in x and y direction
	Direction _facing;
};