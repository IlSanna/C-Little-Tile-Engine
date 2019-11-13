#include "../headers/player.h"

namespace player_constant {
	const float WALK_SPEED = 0.2f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
	const float JUMP_SPEED = 0.7f;
}

Player::Player() {
}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "content/sprite/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 300),//shouldbe 16
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false)
{
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

void Player::jump() {
	if (_grounded) {
		_dy = 0;
		_dy -= player_constant::JUMP_SPEED;
		_grounded = false;
	}
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
		std::cout << std::endl;
		sides::Side collisionSide = getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				std::cout << " TOP ";
				_dy = 0;//reset gravity counter
				_y = others.at(i).getBottom() + 1;//+1;//if we hit the ceiling reset player pos 1 pixel down the bottom of the ceiling
				if (_grounded) {
					_dx = 0;
					_x -= _facing == RIGHT ? 1.0f : -1.0f;
				}
				break;
			case sides::BOTTOM:
				std::cout << " BOTTOM ";
				_dy = 0;
				_y = others.at(i).getTop() - _boundingBox.getHeight() - 1-1;//same as above but one pixel and the player height to go up 
				
				_grounded = true;
				break;
			case sides::LEFT:
				std::cout << " LEFT ";
				/*if (!_grounded) {
					_dy = 0.1f;
				}*/
				_x = others.at(i).getRight() + 1;// +3;
				
				break;
			case sides::RIGHT:
				std::cout << " RIGHT ";
				_x = others.at(i).getLeft() - _boundingBox.getWidth() - 1-3;//same as above but on other side
				
				break;
			}
		}
		
	}
}

void Player::handleSlopeCollision(std::vector<Slope> &others) {
	for (int i = 0; i < others.size(); i++) {
		//Calculate where on the slope the player's bottom center is touching
		//and use y=mx+b to figure out the y position to place him at
		//First calculate "b" (slope intercept) using one of the points (b = y - mx)
		//std::cout << others.at(i).getP1().y << " - ( " << others.at(i).getSlope() << " * " << fabs(others.at(i).getP1().x) << std::endl;

		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		//Now get player's center x
		int centerX = _boundingBox.getCenterX();

		//Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
		int newY = (others.at(i).getSlope() * centerX) + b -16; //8 is temporary to fix a problem
		//std::cout << "newY= "<<newY<<" b = "<<b<<" centerX = "<<centerX << std::endl;
		//Re-position the player to the correct "y"
		if (_grounded) {
			_y = newY - _boundingBox.getHeight();
			_grounded = true;
		}
	}
}

void Player::handleDoorsCollision(std::vector<Door> &others, Level &level, Graphics &graphics) {
	for (int i = 0; i < others.size(); i++) {
		level = Level("content/tileset/PrtCave.png",others.at(i).getDestination(), graphics);
		_x = level.getPlayerSpawnPoint().x;
		_y = level.getPlayerSpawnPoint().y;
	}
}

const float Player::getX() const {
	return _x;
}

const float Player::getY() const {
	return _y;
}

void Player::setGrounded(bool value) {
	_grounded = value;
}
