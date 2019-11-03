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
	const float getX() const;
	const float getY() const;
private:
	float _dx, _dy;//deltaX and deltaY, the change in x and y direction
	Direction _facing;

	bool _grounded;
};