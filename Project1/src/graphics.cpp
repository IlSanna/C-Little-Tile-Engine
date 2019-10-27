#include "../headers/graphics.h"

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &_window, &_renderer);
	SDL_SetWindowTitle(_window, "MyGame");
}

Graphics::~Graphics() {
	SDL_DestroyWindow(_window);
}

SDL_Surface * Graphics::loadImage(const std::string &filePath) {
	if (_spriteSheets.count(filePath) == 0) {//this means it isnt loaded, it cant find it->then load
		_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	else {
		std::cout << "image already in memory" << std::endl;
	}
	return _spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	//this copy a texture to the renderer
	SDL_RenderCopy(_renderer, source, sourceRectangle, destinationRectangle);
}

void Graphics::clear() {
	SDL_RenderClear(_renderer);
}

void Graphics::display() {
	//this actually render to the screen
	SDL_RenderPresent(_renderer);
}

SDL_Renderer * Graphics::getRenderer() const {
	return _renderer;
}
