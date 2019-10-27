#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include "../headers/globals.h"

class Graphics {
public:
	Graphics(); 
	~Graphics();	
	//load an image to the _spritesheets map, if dosnt exists already
	//return that image
	SDL_Surface* loadImage(const std::string &filePath);
	//draw a texture to a certain part of the screen
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
	//render everything to the screen
	void display();
	//clear the previous frame
	void clear();
	//return the renderer
	SDL_Renderer* getRenderer() const;
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;	
	//is going to hold in memory every spriteSheet we loaded
	std::map<std::string, SDL_Surface*> _spriteSheets;
};



