#pragma once
#include "graphics.h"
#include "animatedSprite.h"

class Enemy;

class Whip : public AnimatedSprite {
public:
	Whip();
	Whip(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime, float x, float y,Direction facing);

	virtual void setupAnimation();
	virtual void animationDone(std::string currentAnimation);
	
	void handleEnemyCollisions(std::vector<Enemy*>& others, float elapsedTime);

	void setX(float value);
	void setY(float value);
	void setActive(bool value);
	bool getActive(); 
private:
	float _dx = 4;
	float _dy;//deltaX and deltaY, the change in x and y direction
	double _timeToUpdate = 1000;
	double _timeElapsed = 0;
	Direction _facing;
	Vector2 _lastPos;
	bool _isActive = false;
};
