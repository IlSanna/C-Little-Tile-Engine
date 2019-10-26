#include <SDL.h>
#include "game.h"
#include "graphics.h"

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	gameLoop();
}

Game::~Game() {
}

void Game::gameLoop() {
	Graphics graphics;
	SDL_Event event;

	while (true) {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
		}
	}
}

void Game::update(float elapsedTime) {
}

void Game::draw(Graphics &graphics) {
}

