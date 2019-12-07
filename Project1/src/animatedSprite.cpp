#include "../headers/animatedSprite.h"

AnimatedSprite::AnimatedSprite() {
}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
	int width, int height, float posX, float posY, float timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	_frameIndex(0),
	_timeToUpdate(timeToUpdate),
	_visible(true),
	_currentAnimationOnce(false),
	_currentAnimation(""),
	_timeElapsed(0) {

}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	_currentAnimationOnce = once;
	if (_currentAnimation != animation) {
		_currentAnimation = animation;
		_frameIndex = 0;
	}
}

void AnimatedSprite::update(float elapsedTime) {
	Sprite::update();
	_timeElapsed += elapsedTime;
	if (_timeElapsed > _timeToUpdate) {
		_timeElapsed -= _timeToUpdate;
		if (_frameIndex < _animations[_currentAnimation].size() - 1) {//if is not the last frame of the anim
			_frameIndex++;
		}
		else {
			if (_currentAnimationOnce) {//if it need to be played just one frame
				setVisible(false);
			}
			stopAnimation();
		}
	}
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y) {
	if (_visible) {
		SDL_Rect destinationRectangle;
		if (_currentAnimation == "attack") {
			destinationRectangle.x = x + _offsets[_currentAnimation].x;
			destinationRectangle.y = y + _offsets[_currentAnimation].y;
			destinationRectangle.w = 2*_sourceRect.w * globals::SPRITE_SCALE;
			destinationRectangle.h = _sourceRect.h * globals::SPRITE_SCALE;
		}
		else {
			destinationRectangle.x = x + _offsets[_currentAnimation].x;
			destinationRectangle.y = y + _offsets[_currentAnimation].y;
			destinationRectangle.w = _sourceRect.w * globals::SPRITE_SCALE;
			destinationRectangle.h = _sourceRect.h * globals::SPRITE_SCALE;
		}

		SDL_Rect sourceRect = _animations[_currentAnimation][_frameIndex];
		graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRectangle);
	}
}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> rectangles;
	if (name == "attack") {
		SDL_Rect newRect = { 0 * width, y, width, height };
		rectangles.push_back(newRect);
		SDL_Rect newRect1 = { 1 * width, y, width, height };
		rectangles.push_back(newRect1);
		SDL_Rect newRect2 = { 1* width, y, width, height };
		rectangles.push_back(newRect2);
	}
	else {
		for (int i = 0; i < frames; i++) {
			SDL_Rect newRect = { (i + x)*width, y, width, height };//loop through the sprite sheet row by row
			rectangles.push_back(newRect);//add it to the vector list
		}
	}
	//we add this animation to the list of animations
	_animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
	//add the relative offset of this animation to the list of offsets
	_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::stopAnimation() {
	_frameIndex = 0;
	animationDone(_currentAnimation);
}

void AnimatedSprite::resetAnimation() {
	_animations.clear();
	_offsets.clear();
}

void AnimatedSprite::setVisible(bool visible) {
	_visible = visible;
}

const bool AnimatedSprite::getVisible() const {
	return _visible;
}

void AnimatedSprite::animationDone(std::string currentAnimation) {

}
