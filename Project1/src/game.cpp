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

	_level = Level("content/tileset/PrtCave.png","content/maps/Map1.tmx", graphics);
	_player = Player(graphics, _level.getPlayerSpawnPoint());
	_graphics = graphics;

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
		else if (input.isKeyHeld(SDL_SCANCODE_A)) {
			_player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_D)) {
			_player.moveRight();
		}
		if (input.wasKeyPressed(SDL_SCANCODE_SPACE)) {
			_player.jump();
		}
		if (!input.isKeyHeld(SDL_SCANCODE_A) && !input.isKeyHeld(SDL_SCANCODE_D)) {
			_player.stopMoving();
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
	_player.update(elapsedTime);
	_level.update(elapsedTime);

	//collision check
	std::vector<Rectangle> others;
	std::vector<Slope> otherSlopes;
	std::vector<Door> otherDoors;

	//if the size of the vector others is greater than zero
	//vector created with the check tile collision functions
	if ( (others = _level.checkTileCollision(_player.getBoundingBox()) ).size() > 0) {
		//if we are here means that we are colliding with at least one tile
		_player.handleTileCollision(others);
	}

	//Check slopes
	if ((otherSlopes = _level.checkSlopeCollision(_player.getBoundingBox())).size() > 0) {
		_player.handleSlopeCollision(otherSlopes);
	}
	//Check slopes
	if ((otherDoors = _level.checkDoorsCollision(_player.getBoundingBox())).size() > 0) {
		_player.handleDoorsCollision(otherDoors,_level,_graphics);
	}
	
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	_level.draw(graphics);
	_player.draw(graphics);
	graphics.display();
}

