#include "../headers/whip.h"

Whip::Whip() {
}

Whip::Whip(Graphics & graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "content/sprite/prov.png", 0, 0, 22, 8, spawnPoint.x, spawnPoint.y, 0),//shouldbe 16
	_dx(0),
	_dy(0),
	_facing(RIGHT)
{
	setupAnimation();
	playAnimation("IdleLeft");
}

void Whip::update(float elapsedTime, float x, float y, Direction facing) {
	if (getVisible()) {
		_x = facing == Direction::RIGHT ?
			 x + 16 * globals::SPRITE_SCALE:
			 x - 22 * globals::SPRITE_SCALE;//16-22=6 bb size is 22, so in this case 16+6=22
		_y = y + 8 * globals::SPRITE_SCALE;
		//update BB position
		_boundingBox = Rectangle(
			_x,
			_y,
			(_sourceRect.w - 1) * globals::SPRITE_SCALE,
			(_sourceRect.h - 1)* globals::SPRITE_SCALE
		);

		AnimatedSprite::update(elapsedTime);
	}
	
}
void Whip::draw(Graphics & graphics) {
	AnimatedSprite::draw(graphics, _x, _y);
}
void Whip::setupAnimation() {
	addAnimation(1, 0, 0, "IdleLeft", 22, 8, Vector2(0, 0));
}
void Whip::animationDone(std::string currentAnimation) {
}
void Whip::handleTileCollision(std::vector<Rectangle>& others) {
	for (int i = 0; i < others.size(); i++) {
		std::cout << std::endl;
		sides::Side collisionSide = getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				std::cout << " TOP ";
				_dy = 0;
				_y = others.at(i).getBottom() + 1;
				break;
			case sides::BOTTOM:
				std::cout << " BOTTOM TILE";
				_y = others.at(i).getTop() - _boundingBox.getHeight() - 1;
				break;
			case sides::LEFT:
				std::cout << " LEFT TILE";
				_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				std::cout << " RIGHT TILE";
				_x = others.at(i).getLeft() - _boundingBox.getWidth() - 1;
				break;
			}
		}
	}
}
void Whip::setX(float value) {
	_x = value;
}
void Whip::setY(float value) {
	_y = value;
}
