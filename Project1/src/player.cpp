#include "../headers/player.h"

namespace player_constant {
	const float WALK_SPEED = 0.2f;
}

Player::Player() {
}

Player::Player(Graphics &graphics, float x, float y) :
	AnimatedSprite(graphics, "content/sprite/MyChar.png", 0, 0, 16, 16, x, y, 300) {

	//graphics.loadImage("content/sprite/MyChar.png");
	setupAnimation();
	playAnimation("RunLeft");
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics,_x,_y);//this _x and _y came from sprite.h
}

void Player::update(float elapsedTime) {
	_x += _dx * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}

void Player::moveLeft() {
	_dx = -player_constant::WALK_SPEED;
	playAnimation("RunLeft");
	_facing = LEFT; 
}

void Player::moveRight() {
	_dx = player_constant::WALK_SPEED;
	playAnimation("RunRight");
	_facing = RIGHT;
}

void Player::stopMoving() {
	_dx = 0.0f;
	playAnimation(_facing == RIGHT ? "IdleRight" : "IdleLeft");
}

void Player::setupAnimation() {
	addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {
}
