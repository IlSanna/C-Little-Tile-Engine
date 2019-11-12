#pragma once
#include "graphics.h"
#include "Tile.h"
#include <vector>
#include <sstream>
#include "tinyxml2.h"
#include <fstream>
#include "rectangle.h"
#include "slope.h"
#include "door.h"

struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;
	Tileset(SDL_Texture* texture, int firstGid) {
		Texture = texture;
		FirstGid = firstGid;
	}

};

class Level {
public:
	Level();
	Level(std::string tilesetPath,std::string mapNameInfo, Graphics &graphics);

	void update(float elapsedTime);
	void draw(Graphics &graphics);

	std::vector<Rectangle> checkTileCollision(const Rectangle &other);
	std::vector<Slope> checkSlopeCollision(const Rectangle &other);
	std::vector<Door> checkDoorsCollision(const Rectangle &other);

	const Vector2 getPlayerSpawnPoint() const;
private:
	std::string _tilesetPath;
	std::string _mapNameInfo;
	Vector2 _spawnPoint;

	Vector2 _size;//width and height of the entire map
	Vector2 _tileSize;
	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tileSets;
	std::vector<Rectangle> _collisionRect;
	std::vector<Slope> _collisionSlopes;
	std::vector<Door> _doorList;

	void extractTileInfo(tinyxml2::XMLElement* pData, SDL_Texture* tileset);
	void setTile(SDL_Texture * tileset, int currentGid, const Vector2 &finalTilePosition);
	void loadTiledObjects(tinyxml2::XMLElement * pObjectGroup);
	void addCollisionRectangle(tinyxml2::XMLElement * pObject);
	void loadMapInfo(std::string mapName, SDL_Texture* tileset, Graphics &graphics);
};

