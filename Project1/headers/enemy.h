#pragma once
#include "animatedsprite.h"
#include "globals.h"
#include "player.h"


class Enemy: public AnimatedSprite {
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, 
		int width, int height,Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	virtual void touchPlayer(Player* player) = 0;
	virtual void takeDamage(int value);

	const inline int getMaxHealth() const { return _maxHealth; }
	const inline int getCurrentHealth() const { return _currentHealth; }

	const bool getVulnerability();
	const bool getDie();
protected:
	Direction _direction;
	int _maxHealth;
	int _currentHealth;

	bool _isVulnerable = true;
	Timer* _invincibilityTimer;
	float _lastHitTime = 0;

	bool _die;
};

class Bat : public Enemy {
public:
	Bat();
	Bat(Graphics &graphics, Vector2 spawnPoint);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);
	void takeDamage(int value);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimation();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
};