#include "../headers/player.h"

namespace player_constant {
	const float WALK_SPEED = 0.2f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player() {
}

Player::Player(Graphics &graphics, float x, float y) :
	AnimatedSprite(graphics, "content/sprite/MyChar.png", 0, 0, 16, 16, x, y, 300),//shouldbe 16
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false)
{

	//graphics.loadImage("content/sprite/MyChar.png");
	setupAnimation();
	playAnimation("RunLeft");
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics,_x,_y);//this _x and _y came from sprite.h
}

void Player::update(float elapsedTime) {
	//apply gravity
	if (_dy <= player_constant::GRAVITY_CAP) {
		_dy += player_constant::GRAVITY * elapsedTime;
	}
	//move by deltax
	_x += _dx * elapsedTime;
	_y += _dy * elapsedTime;
	//move by deltay
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

void Player::handleTileCollision(std::vector<Rectangle>& others) {
	//look for which side is the player colliding with and move accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				_y = others.at(i).getBottom() + 1;//if we hit the ceiling reset player pos 1 pixel down the bottom of the ceiling
				_dy = 0;//reset gravity counter 
				break;
			case sides::BOTTOM:
				_y = others.at(i).getTop() - _boundingBox.getHeight() - 1;//same as above but one pixel and the player height to go up 
				_dy = 0;
				_grounded = true;
				break;
			case sides::LEFT:
				_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				_x = others.at(i).getLeft() - _boundingBox.getWidth() - 1;//same as above but on other side
				break;
			}
		}
	}
}

const float Player::getX() const {
	return _x;
}

const float Player::getY() const {
	return _y;
}
