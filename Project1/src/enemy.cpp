#include "../headers/enemy.h"

//Base enemy class
Enemy::Enemy() {}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
		spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(0),
	_currentHealth(0) {
}

void Enemy::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, _x, _y);
}

//Bat class
Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "content/tileset/enemies.png", 0, 0, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false) {
	setupAnimation();
	playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player &player) {
	_direction = player.getX() > _x ? RIGHT : LEFT;
	playAnimation(_direction == RIGHT ? "FlyRight" : "FlyLeft");

	//Move up or down
	_y += _shouldMoveUp ? -.05 : .05;
	if (_y > (_startingY + 40) || _y < _startingY - 40) {
		_shouldMoveUp = !_shouldMoveUp;
	}

	//update BB position
	_boundingBox = Rectangle(//magic numbers to make a better BB
		_x,// +3,
		_y,// +1, 
		(_sourceRect.w - 1) * globals::SPRITE_SCALE,//-3
		(_sourceRect.h - 1) * globals::SPRITE_SCALE
	);

	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation) {

}

void Bat::setupAnimation() {
	addAnimation(3, 6, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	addAnimation(3, 6, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Bat::touchPlayer(Player* player) {
	//player->gainHealth(-1);
}
