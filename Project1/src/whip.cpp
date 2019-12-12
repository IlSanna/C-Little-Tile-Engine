#include "../headers/whip.h"
#include "../headers/enemy.h"
Whip::Whip() {
}

Whip::Whip(Graphics & graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "content/sprite/whip.png", 0, 0, 23, 8, spawnPoint.x, spawnPoint.y, 0),//shouldbe 16
	_dx(0),
	_dy(0),
	_facing(RIGHT)
{
	setVisible(false);
	setupAnimation();
	playAnimation("Right");
}

void Whip::update(float elapsedTime, float x, float y, Direction facing) {
	if (_isActive) {
		if (facing == Direction::RIGHT) {
			_x = x + 14 * globals::SPRITE_SCALE;
			playAnimation("Right");
		}
		else {
			_x = x - 23 * globals::SPRITE_SCALE;
			playAnimation("Left");
		}
		_y = y + 8 * globals::SPRITE_SCALE;
		//update BB position
		_boundingBox = Rectangle(
			_x,
			_y,
			(_sourceRect.w - 1) * globals::SPRITE_SCALE,
			(_sourceRect.h - 1)* globals::SPRITE_SCALE
		);
		_timeElapsed += elapsedTime;
		if (_timeElapsed > 1000 / 4) {//6 is firerate
			_timeElapsed = 0;
			_isActive = false;
			setVisible(false);
		}
	}
}
void Whip::draw(Graphics & graphics) {
	AnimatedSprite::draw(graphics, _x, _y);
}
void Whip::setupAnimation() {
	addAnimation(1, 0, 0, "Right", 46, 16, Vector2(0, 0));
	addAnimation(1, 0, 16, "Left", 46, 16, Vector2(0, 0));
}
void Whip::animationDone(std::string currentAnimation) {
}
void Whip::handleEnemyCollisions(std::vector<Enemy*>& others, float elapsedTime) {
	for (int i = 0; i < others.size(); i++) {
		if (others.at(i)->getVulnerability()) {
			others.at(i)->takeDamage(1);
		}
	}
}
void Whip::setX(float value) {
	_x = value;
}
void Whip::setY(float value) {
	_y = value;
}

void Whip::setActive(bool value) {
	if (value) {
		setVisible(true);
		_isActive = value;
	}
	else {
		_isActive = value;
	}
}
bool Whip::getActive() {
	return _isActive;
}
