#include "../headers/player.h"

namespace player_constant {
	const float WALK_SPEED = 0.14f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
	const float JUMP_SPEED = 0.5f;
	const int WALKABLE_STEP = 1;
}

Player::Player() {
}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "content/sprite/simonTemplate.png", 0, 0, 16, 32, spawnPoint.x, spawnPoint.y, 160),//shouldbe 16
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),_wantsToJump(false)
{
	setupAnimation();
	playAnimation("RunLeft");

	_whip = Whip(graphics, Vector2(_x+32, _y));
	//_whip.setVisible(false);
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
		_grounded = false;
		_wantsToJump = true;
		_dy = 0;
		_dy -= player_constant::JUMP_SPEED;
		
		std::cout << "jump " << _grounded<< std::endl;
	}
}

void Player::setupAnimation() {
	addAnimation(1, 0, 0, "IdleLeft", 16, 32, Vector2(0, 0));
	addAnimation(1, 0, 32, "IdleRight", 16, 32, Vector2(0, 0));
	addAnimation(3, 0, 0, "RunLeft", 16, 32, Vector2(0, 0));
	addAnimation(3, 0, 32, "RunRight", 16, 32, Vector2(0, 0));
	addAnimation(3, 0, 64, "attack", 32, 32, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {
}
void Player::update(float elapsedTime) {
	//apply gravity
	if (_dy <= player_constant::GRAVITY_CAP) {
		_dy += player_constant::GRAVITY * elapsedTime;
	}
	//move by deltax
	_x += _dx * elapsedTime;
	_y += _dy * elapsedTime;

	//groundcheck
	_grounded = (int)_y == _lastPos.y ? true : false;
	
	//update BB position
	_boundingBox = Rectangle(//magic numbers to make a better BB
		_x,// +3,
		_y,// +1, 
		(_sourceRect.w - 1) * globals::SPRITE_SCALE,//-3
		(_sourceRect.h - 2)* globals::SPRITE_SCALE
	);
	
	AnimatedSprite::update(elapsedTime);

	//_timeElapsed += elapsedTime;
	//std::cout <<"vulnerable = "<<_isVulnerable<< " health: " << _health << std::endl;
	_whip.update(elapsedTime, _x , _y,_facing);
}
void Player::handleTileCollision(std::vector<Rectangle>& others) {
	//look for which side is the player colliding with and move accordingly
	for (int i = 0; i < others.size(); i++) {
		//std::cout << std::endl;
		sides::Side collisionSide = getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				//std::cout << " TOP ";
				_dy = 0;//reset gravity counter
				_y = others.at(i).getBottom() + 1;//+1;//if we hit the ceiling reset player pos 1 pixel down the bottom of the ceiling
				if (_grounded) {
					_dx = 0;
					_x -= _facing == RIGHT ? 1.0f : -1.0f;
				}
				break;
			case sides::BOTTOM:
				//std::cout << " BOTTOM TILE";
				_dy = 0;
				_y = others.at(i).getTop() - _boundingBox.getHeight() -1;// -1;//same as above but one pixel and the player height to go up 
				//_grounded = true;
				_lastPos = Vector2(_x, _y);
				break;
			case sides::LEFT:
				//std::cout << " LEFT TILE";
				//std::cout << others.at(i).getTop() << " - " << ((int)_y + _boundingBox.getHeight())
					//<< " = " << others.at(i).getTop() - ((int)_y + _boundingBox.getHeight()) << std::endl;
				/*if (others.at(i).getTop() - ((int)_y + _boundingBox.getHeight()) <
					player_constant::WALKABLE_STEP) {
					_y -= 1;
				}*/
				if (others.at(i).getTop() - ((int)_y + _boundingBox.getHeight()) == 0) {
					_y -= 1;
				}
				_x = others.at(i).getRight() + 1;// +3;
				
				break;
			case sides::RIGHT:
				//std::cout << " RIGHT TILE";
				//std::cout << others.at(i).getTop() <<" - "<< ((int)_y + _boundingBox.getHeight()) 
					//<<" = "<< others.at(i).getTop() - ((int)_y + _boundingBox.getHeight()) << std::endl;

				//if player pos minus top tile is less than walkable step
				if (others.at(i).getTop() - ((int)_y + _boundingBox.getHeight()) == 0) {
					_y -= 1;
				}
				_x = others.at(i).getLeft() - _boundingBox.getWidth() - 1;// -3;//same as above but on other side
				break;
			}
		}
		
	}
}
void Player::handleSlopeRectCollision(std::vector<Rectangle>& others) {
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = getCollisionSide(others.at(i));
		if (collisionSide != sides::TOP) {
			
			if (_grounded && !_wantsToJump) {//_grounded && !_wantsToJump
				_dy = 0;
				if (!others.at(i).isLeftSlope()) {//right slope
					_y = others.at(i).getTop() - _boundingBox.getHeight() - (_x - others.at(i).getLeft()) +2;
				}
				else {//left slope
					_y = (others.at(i).getTop() - _boundingBox.getHeight() -
						(2 - (_x - others.at(i).getLeft()))) + 1;
				}
				//_lastPos.y = _y;
			}
			_lastPos.y = _y;
			//_grounded = true;
		}
		else {
			_dy = 0;//reset gravity counter
			_y = others.at(i).getBottom() + 1;//+1;//if we hit the ceiling reset player pos 1 pixel down the bottom of the ceiling
			if (_grounded) {
				_dx = 0;
				_x -= _facing == RIGHT ? 1.0f : -1.0f;
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
		level = Level(others.at(i).getDestination(), graphics);//"content/tileset/ClockTowerTileset.png",
		_x = level.getPlayerSpawnPoint().x;
		_y = level.getPlayerSpawnPoint().y;
	}
}
void Player::handleEnemyCollisions(std::vector<Enemy*>& others,float elapsedTime) {
	std::cout << "vulnerable = " << _isVulnerable << " health: " << _health << std::endl;
	_isVulnerable = false;
	if (!_isVulnerable) {//potrei tenere un timer a parte, che mi tiene vulnerable
		//consigliano di avere un timer che va decrementando, se è diverso da zero allora è vulnerabile
		//http://lazyfoo.net/SDL_tutorials/lesson12/index.php
		std::cout << "ai" << std::endl;
		_health--;
		//for (int i = 0; i < others.size(); i++) {
		//	std::cout << "ai" << std::endl;
		//	//resetta il timer
		//	//_timeElapsed = 0;
		//	//logica health
		//	_health--;
		//	//others.at(i)->touchPlayer(this);
		//}
	}
	_isVulnerable = true;
	
	std::cout << "vulnerable = " << _isVulnerable << " health: " << _health << std::endl;
}

const float Player::getX() const {
	return _x;
}

const float Player::getY() const {
	return _y;
}

Whip Player::getWhip() {
	return _whip;
}

void Player::setWantsToJump(bool value) {
	_wantsToJump = value;
}

void Player::attack() {
	playAnimation("attack");
	_whip.setActive(true);
}

void Player::draw(Graphics &graphics) {
	
	AnimatedSprite::draw(graphics, _x, _y);//this _x and _y came from sprite.h
	_whip.draw(graphics);
}