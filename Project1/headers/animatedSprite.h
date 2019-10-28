#pragma once
#include "sprite.h"
#include <vector>

class AnimatedSprite : public Sprite{
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
		int width, int height, float posX, float posY, float timeToUpdate);
	void playAnimation(std::string animation, bool once = false);//optional parameter syntax
	void update(float elapsedTime);
	void draw(Graphics &graphics, int x, int y);

protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	//add a sprite to the map
	void addAnimation(int frames,int x, int y, std::string name, int width, int height, Vector2 offset);
	void stopAnimation();
	//remove all sprite from the map
	void resetAnimation();
	void setVisible(bool visible);
	//set each animation
	virtual void setupAnimation() = 0;
	//logic that happens when an animation end
	virtual void animationDone(std::string currentAnimation) = 0;
private:
	std::map<std::string, std::vector<SDL_Rect>> _animations;
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;//wich animations we are at
	double _timeElapsed;//for the timer
	bool _visible;
};