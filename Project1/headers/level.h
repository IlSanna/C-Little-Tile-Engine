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

class Enemy;
class Player;

struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;
	Tileset(){}
	Tileset(SDL_Texture* texture, int firstGid) {
		Texture = texture;
		FirstGid = firstGid;
	}

};

class Level {
public:
	Level();
	Level(std::string mapNameInfo, Graphics &graphics);//std::string tilesetPath,

	void update(float elapsedTime, Player &player);
	void draw(Graphics &graphics);

	std::vector<Rectangle> checkTileCollision(const Rectangle &other);
	std::vector<Rectangle> checkSlopeRectCollision(const Rectangle & other);
	std::vector<Slope> checkSlopeCollision(const Rectangle &other);
	std::vector<Door> checkDoorsCollision(const Rectangle &other);

	const Vector2 getPlayerSpawnPoint() const;
private:
	//std::string _tilesetPath;
	std::string _mapNameInfo;
	Vector2 _spawnPoint;

	Vector2 _size;//width and height of the entire map
	Vector2 _tileSize;
	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tileSets;
	std::vector<Rectangle> _collisionRect;
	std::vector<Rectangle> _slopesRect;
	std::vector<Slope> _collisionSlopes;
	std::vector<Door> _doorList;
	std::vector<Enemy*> _enemiesList;

	void extractTileInfo(tinyxml2::XMLElement* pData, std::vector<Tileset> tilesets);
	void setTile(Tileset tileset, int currentGid, const Vector2 &finalTilePosition);
	void loadTiledObjects(tinyxml2::XMLElement * pObjectGroup, Graphics &graphics);
	void addCollisionRectangle(tinyxml2::XMLElement * pObject);
	void addSlopeRectangle(tinyxml2::XMLElement * pObject, std::string value);
	void loadMapInfo(std::string mapName, Graphics &graphics);//, SDL_Texture* tileset
};

