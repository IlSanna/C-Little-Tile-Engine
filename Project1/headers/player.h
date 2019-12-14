#pragma once
#include "graphics.h"
#include "animatedSprite.h"
#include "level.h"
#include "whip.h"
#include "Timer.h"

class AudioManager;

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
	void handleDoorsCollision(std::vector<Door> &others, Level &level, Graphics &graphics);
	void handleEnemyCollisions(std::vector<Enemy*>& others, float elapsedTime);

	const float getX() const;
	const float getY() const;

	Whip getWhip();
	void setWantsToJump(bool value);
	void attack();

	bool getVulnerability(); 
private:
	float _dx, _dy;//deltaX and deltaY, the change in x and y direction
	Direction _facing;
	Vector2 _lastPos;
	bool _wantsToJump;
	bool _grounded = false;
	Whip _whip;

	int _health = 3;

	bool _isVulnerable = true;
	Timer* _invincibilityTimer;
	float _lastHitTime = 0;

	bool _isAttacking;
	AudioManager* _audioManager;
};