#include "../headers/player.h"
#include "../headers/AudioManager.h"

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
	AnimatedSprite(graphics, "content/sprite/simonFinal.png", 0, 0, 16, 32, spawnPoint.x, spawnPoint.y, 160),//shouldbe 16
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_wantsToJump(false),
	_lastHitTime(0),
	_isAttacking(false)
{
	setupAnimation();
	playAnimation("RunLeft");

	_whip = Whip(graphics, Vector2(_x+32, _y));

	_invincibilityTimer = Timer::Instance();
	_audioManager = AudioManager::Instance();
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
	if (!_isAttacking) {
		playAnimation(_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::jump() {
	
	if (_grounded) {
		_grounded = false;
		//_wantsToJump = true;
		_dy = 0;
		_dy -= player_constant::JUMP_SPEED;
		_audioManager->PlaySFX("content/music/jump.wav");
	}
}

void Player::setupAnimation() {
	addAnimation(1, 0, 0, "IdleLeft", 16, 32, Vector2(0, 0));
	addAnimation(1, 0, 32, "IdleRight", 16, 32, Vector2(0, 0));
	addAnimation(3, 0, 0, "RunLeft", 16, 32, Vector2(0, 0));
	addAnimation(3, 0, 32, "RunRight", 16, 32, Vector2(0, 0));
	addAnimation(3, 0, 64, "LeftAttack", 24, 32, Vector2(-8, 0));
	addAnimation(3, 0, 96, "RightAttack", 24, 32, Vector2(-8, 0));
}

void Player::animationDone(std::string currentAnimation) {
	//fare uno switch qua
	/*if (currentAnimation == "RightAttack") {
		_whip.setActive(true);

	}*/
	_isAttacking = false;
}

void Player::update(float elapsedTime) {
	_invincibilityTimer->Update();

	//invulnerability
	if (_invincibilityTimer->DeltaTime() - _lastHitTime > 1) {
		SDL_SetTextureAlphaMod(_spriteSheet, Uint8(255));
		if ((_invincibilityTimer->DeltaTime() - _lastHitTime > 1.5f)) {
			_isVulnerable = true;
		}
	}
	//apply gravity
	if (_dy <= player_constant::GRAVITY_CAP) {
		_dy += player_constant::GRAVITY * elapsedTime;
	}
	//move by deltax
	_x += _dx * elapsedTime;
	_y += _dy * elapsedTime;

	//groundcheck
	_grounded = (int)_y == _lastPos.y ? true : false;
	
	_wantsToJump = _dy < 0 ? true : false;//forse torna al key release
	
	//update BB position
	_boundingBox = Rectangle(//magic numbers to make a better BB
		_x,// +3,
		_y,// +1, 
		(_sourceRect.w - 1) * globals::SPRITE_SCALE,//-3
		(_sourceRect.h - 2)* globals::SPRITE_SCALE
	);
	
	AnimatedSprite::update(elapsedTime);
	
	if (!_isVulnerable) {
		//flicker
	}

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
			if (_grounded && !_wantsToJump) {//grounded, handle on slope movement && !_wantsToJump
				_dy = 0;
				if (!others.at(i).isLeftSlope()) {//right slope
					_y = others.at(i).getTop() - _boundingBox.getHeight() 
						- (_x - others.at(i).getLeft()) +2;
				}
				else {//left slope
					_y = (others.at(i).getTop() - _boundingBox.getHeight() 
						- (2 - (_x - others.at(i).getLeft()))) + 1;
				}
			}
			_lastPos.y = _y;//se tocco la slope, salvo la last pos FORSE ERRORE
		}
		else {//im colliding with the slope top, so use standard collision response
			_dy = 0;//reset gravity counter
			_y = others.at(i).getBottom() + 1;//+1;//if we hit the ceiling reset player pos 1 pixel down the bottom of the ceiling
			if (_grounded) {
				_dx = 0;
				_x -= _facing == RIGHT ? 1.0f : -1.0f;
			}
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
	_isVulnerable = false;
	_lastHitTime = _invincibilityTimer->DeltaTime();
	
	for (int i = 0; i < others.size(); i++) {
		_audioManager->PlaySFX("content/music/Being_Hit.wav");
		_health--;
	}
	std::cout << "health: " << _health << std::endl;
	SDL_SetTextureAlphaMod(_spriteSheet, Uint8(175));
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
	_isAttacking = true;
	//playAnimation(_facing == RIGHT ? "RightAttack" : "LeftAttack");
	_audioManager->PlaySFX("content/music/attack.wav");
	_whip.setActive(true);
}

bool Player::getVulnerability() {
	return _isVulnerable;
}

void Player::draw(Graphics &graphics) {
	
	AnimatedSprite::draw(graphics, _x, _y);//this _x and _y came from sprite.h
	_whip.draw(graphics);
}