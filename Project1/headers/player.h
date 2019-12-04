#pragma once
#include "graphics.h"
#include "animatedSprite.h"
#include "slope.h"
#include "level.h"

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	void moveLeft();
	void moveRight();
	void stopMoving();
	void jump();

	virtual void setupAnimation();
	virtual void animationDone(std::string currentAnimation);

	void handleTileCollision(std::vector<Rectangle> &others);
	void handleSlopeRectCollision(std::vector<Rectangle>& others);
	void handleSlopeCollision(std::vector<Slope> &others);
	void handleDoorsCollision(std::vector<Door> &others, Level &level, Graphics &graphics);

	const float getX() const;
	const float getY() const;
	void setGrounded(bool value);
	void setWantsToJump(bool value);
private:
	float _dx, _dy;//deltaX and deltaY, the change in x and y direction
	Direction _facing;
	Vector2 _lastPos;
	bool _wantsToJump;
	bool _grounded = false;
};