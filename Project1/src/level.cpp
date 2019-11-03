#include "../headers/level.h"

using namespace tinyxml2;

Level::Level() {
}

Level::Level(std::string tilesetPath, std::string mapNameInfo, Vector2 spawnPoint, Graphics &graphics) :
	_tilesetPath(tilesetPath),
	_mapNameInfo(mapNameInfo),
	_spawnPoint(spawnPoint),
	_size(Vector2(0,0)) 
{
	SDL_Texture* tileset = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(_tilesetPath));
	loadMapInfo(_mapNameInfo,tileset, graphics);
}

std::vector<Rectangle> Level::checkTileCollision(const Rectangle &other) {
	std::vector<Rectangle> collisions;
	for (int i = 0; i < _collisionRect.size(); i++) {//loop throug the vector of collision extracted from file
		if (_collisionRect.at(i).collideWith(other)) {//if other is colliding with one of them
			collisions.push_back(_collisionRect.at(i));//add it to the collision list
		}
	}
	return collisions;
}

void Level::loadMapInfo(std::string mapName, SDL_Texture* tileset, Graphics &graphics) {
	XMLDocument doc;
	doc.LoadFile(mapName.c_str());
	
	XMLElement* mapNode = doc.FirstChildElement("map");

	//get map height and width
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	_size = Vector2(width, height);

	//get tilewidth and height
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	_tileSize = Vector2(tileWidth, tileHeight);

	////Loading the tilesets
	//XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	//if (pTileset) {
	//	while (pTileset) {//reads until next tileset tag is found
	//		int firstgid;
	//		pTileset->QueryIntAttribute("firstgid", &firstgid);
	//		SDL_Texture* tex = SDL_CreateTextureFromSurface(
	//			graphics.getRenderer(), graphics.loadImage("content/tileset/PrtCave.png"));//attenzione qui con i percorsi
	//		this->_tileSets.push_back(Tileset(tex, firstgid));
	//		pTileset = pTileset->NextSiblingElement("tileset");
	//	}
	//}

	//loading the layer and tiles
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer) {
		while (pLayer) {
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData) {//load the tiles for this layer
				extractTileInfo(pData,tileset);//forse cambiero il tileset che vado passando
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
	//load collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup) {
		loadCollisions(pObjectGroup);//rifare la struttura in modo che legga anche altri object group
	}
}

void Level::extractTileInfo(tinyxml2::XMLElement * pData, SDL_Texture* tileset) {
	std::string temp = pData->GetText();
	std::istringstream split(temp);
	std::string line;
	int tileCounter = 0;
	while (getline(split, line, ',')) {//read the level gids
		int currentGid = stoi(line);
		int xx = 0;
		int yy = 0;
		xx = tileCounter % _size.x;
		xx *= _tileSize.x;
		yy += _tileSize.y * (tileCounter / _size.x);
		Vector2 finalTilePosition = Vector2(xx, yy);
		if (currentGid != 0) {
			setTile(tileset, currentGid, finalTilePosition);
			tileCounter++;
		}
		else {
			tileCounter++;
		}
	}
}

void Level::setTile(SDL_Texture * tileset, int currentGid, const Vector2 &finalTilePosition) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tileset, NULL, NULL, &tilesetWidth, &tilesetHeight);

	int tsxx = currentGid % (tilesetWidth / _tileSize.x) - 1;
	tsxx *= _tileSize.x;
	int tsyy = 0;
	int amt = (currentGid / (tilesetWidth / _tileSize.x));
	tsyy = _tileSize.y * amt;

	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	Tile tile(tileset, Vector2(_tileSize.x, _tileSize.y), finalTilesetPosition, finalTilePosition);
	_tileList.push_back(tile);
}

void Level::loadCollisions(tinyxml2::XMLElement * pObjectGroup) {
	while (pObjectGroup) {//uindi non farlo con un while ma solo con il controllo del nome del object group
		XMLElement* pObject = pObjectGroup->FirstChildElement("object");
		if (pObject) {
			while (pObject) {
				float x, y, width, height;

				x = pObject->FloatAttribute("x");
				y = pObject->FloatAttribute("y");
				width = pObject->FloatAttribute("width");
				height = pObject->FloatAttribute("height");

				_collisionRect.push_back(Rectangle(//ceil will round up the floats
					std::ceil(x) * globals::SPRITE_SCALE, 
					std::ceil(y) * globals::SPRITE_SCALE,
					std::ceil(width) * globals::SPRITE_SCALE,
					std::ceil(height) * globals::SPRITE_SCALE
				));

				pObject = pObject->NextSiblingElement("object");
			}
		}
		pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
	}
}

void Level::update(float elapsedTime) {
}

void Level::draw(Graphics &graphics) {
	for (int i = 0; i < _tileList.size(); i++) {
		_tileList.at(i).draw(graphics);
	}
}