#include "../headers/game.h"

namespace {
	const int FPS = 50;
	//maximum amount of time a frame is allowed to last
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}


Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	gameLoop();
}

Game::~Game() {
}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	_player = Sprite(graphics,"content/sprite/MyChar.png",0,0,16,16,100,100);

	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (true) {

		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {//make sure your are not holding down the key
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
			return;
		}
		const int CURRENT_TIME_MS = SDL_GetTicks();
		//take the time of the last loop and subtracting it
		//from current time i get elapsed time
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		//i have to pass to update the elapsed time and this value need to be
		//bounded between elapsed time and max frame time
		update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS; 
		draw(graphics);
	}
}

void Game::update(float elapsedTime) {
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	_player.draw(graphics, 100, 100);
	graphics.display();
}

