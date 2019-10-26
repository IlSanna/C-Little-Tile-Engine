#include <SDL.h>
#include "graphics.h"

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(640, 480, 0, &_window, &_renderer);
	SDL_SetWindowTitle(_window, "MyGame");
}

Graphics::~Graphics() {
	SDL_DestroyWindow(_window);
}
