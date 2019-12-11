#include "../headers/level.h"

#include "../headers/enemy.h"

using namespace tinyxml2;

Level::Level() {
}

Level::Level(std::string mapNameInfo, Graphics &graphics) ://std::string tilesetPath
	//_tilesetPath(tilesetPath),
	_mapNameInfo(mapNameInfo),
	_size(Vector2(0,0)) 
{
	//SDL_Texture* tileset = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(_tilesetPath));
	loadMapInfo(_mapNameInfo, graphics);//,tileset
}

//returns a vector of collision rectangles based on the precompiled level collisions
std::vector<Rectangle> Level::checkTileCollision(const Rectangle &other) {
	std::vector<Rectangle> collisions;
	for (int i = 0; i < _collisionRect.size(); i++) {//loop throug the vector of collision extracted from file
		if (_collisionRect.at(i).collideWith(other)) {//if other is colliding with one of them
			collisions.push_back(_collisionRect.at(i));//add it to the collision list
		}
	}
	return collisions;
}
std::vector<Rectangle> Level::checkSlopeRectCollision(const Rectangle &other) {
	std::vector<Rectangle> collisions;
	for (int i = 0; i < _slopesRect.size(); i++) {//loop throug the vector of collision extracted from file
		if (_slopesRect.at(i).collideWith(other)) {//if other is colliding with one of them
			collisions.push_back(_slopesRect.at(i));//add it to the collision list
		}
	}
	return collisions;
}
std::vector<Enemy*> Level::checkEnemyCollision(const Rectangle& other) {
	std::vector<Enemy*> collisions;
	for (int i = 0; i < _enemiesList.size(); i++) {
		if (_enemiesList.at(i)->getBoundingBox().collideWith(other)) {//if other is colliding with one of them
			collisions.push_back(_enemiesList.at(i));//add it to the collision list
		}
	}
	return collisions;
}
std::vector<Slope> Level::checkSlopeCollision(const Rectangle & other) {
	std::vector<Slope> collisions;
	for (int i = 0; i < _collisionSlopes.size(); i++) {//loop throug the vector of collision extracted from file
		if (_collisionSlopes.at(i).collideWith(other)) {//if other is colliding with one of them
			collisions.push_back(_collisionSlopes.at(i));//add it to the collision list
		}
	}
	return collisions;
}
std::vector<Door> Level::checkDoorsCollision(const Rectangle &other) {
	std::vector<Door> others;
	for (int i = 0; i < _doorList.size(); i++) {//loop throug the vector of collision extracted from file
		if (_doorList.at(i).collideWith(other)) {//if other is colliding with one of them
			others.push_back(_doorList.at(i));//add it to the collision list
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return _spawnPoint;
}

//load graphic info
void Level::loadMapInfo(std::string mapName, Graphics &graphics) {//, SDL_Texture* tileset
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

	//to add a tileset just copy the line above and look at the first gid number
	SDL_Texture* tilesetTexture0 = SDL_CreateTextureFromSurface(graphics.getRenderer(), 
		graphics.loadImage("content/tileset/ClockTowerTileset.png"));
	_tileSets.push_back(Tileset(tilesetTexture0, 1));

	SDL_Texture* tilesetTexture1 = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage("content/tileset/enemies.png"));
	_tileSets.push_back(Tileset(tilesetTexture1, 837));

	//loading the layer and tiles
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer) {
		while (pLayer) {
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData) {//load the tiles for this layer
				extractTileInfo(pData,_tileSets);
			}
			pLayer = pLayer->NextSiblingElement("layer");
		} 
	}
	//load collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup) {
		loadTiledObjects(pObjectGroup,graphics);
	}
}

//parse the pdata element to find the gid number, then
//based on that it finds the correct world position
//then it created the tile object
void Level::extractTileInfo(tinyxml2::XMLElement * pData, std::vector<Tileset> tilesets) {
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
			Tileset tls;
			int closest = 0;
			for (int i = 0; i < _tileSets.size(); i++) {
				if (_tileSets[i].FirstGid <= currentGid) {
					if (_tileSets[i].FirstGid > closest) {
						closest = _tileSets[i].FirstGid;
						tls = _tileSets.at(i);
					}
				}
			}
			setTile(tls, currentGid, finalTilePosition);
			tileCounter++;
		}
		else {
			tileCounter++;
		}
	}
}

//finds the position of the specific gid on the tileset and 
//create a tile object with the given world position
void Level::setTile(Tileset tileset, int currentGid, const Vector2 &finalTilePosition) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tileset.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int temp = currentGid - tileset.FirstGid;

	int tsxx = temp % (tilesetWidth / _tileSize.x);
	tsxx *= _tileSize.x;
	int tsyy = 0;
	int amt = (temp / (tilesetWidth / _tileSize.x));
	tsyy = _tileSize.y * amt;
	
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	Tile tile(tileset.Texture, Vector2(_tileSize.x, _tileSize.y), finalTilesetPosition, finalTilePosition);
	_tileList.push_back(tile);
}
//END of loading graphic informations

//parse each objets layer from tiled xml
//different behaviours depending on the objects names
void Level::loadTiledObjects(tinyxml2::XMLElement * pObjectGroup,Graphics &graphics) {
	while (pObjectGroup) {//loop through each object groups
		const char* name = pObjectGroup->Attribute("name");
		std::stringstream ss;
		ss << name;
		if (ss.str() == "CollidableTiles") {//if is collision group
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject) {
				while (pObject) {//loop through each object
					addCollisionRectangle(pObject);
					pObject = pObject->NextSiblingElement("object");
				}
			}
		}
		if (ss.str() == "SlopeTiles") {//if is collision group
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject) {
				while (pObject) {//loop through each object


					XMLElement* pProperties = pObject->FirstChildElement("properties");
					if (pProperties) {
						while (pProperties) {
							XMLElement* pProperty = pProperties->FirstChildElement("property");
							if (pProperty) {
								while (pProperty) {
									std::string value = pProperty->Attribute("value");
									addSlopeRectangle(pObject,value);

									pProperty = pProperty->NextSiblingElement("property");
								}
							}
							pProperties = pProperties->NextSiblingElement("properties");
						}
					}



					
					pObject = pObject->NextSiblingElement("object");
				}
			}
		}
		if (ss.str() == "SpawnPoint") {//if is a spawn point group
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject) {
				while (pObject) {//loop through each object
					float x = pObject->FloatAttribute("x");
					float y = pObject->FloatAttribute("y");

					_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
										  std::ceil(y) * globals::SPRITE_SCALE
					);
					pObject = pObject->NextSiblingElement("object");
				}
			}
		}
		if (ss.str() == "Enemies") {//if is a spawn point group
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject) {
				while (pObject) {//loop through each object
					float x = pObject->FloatAttribute("x");
					float y = pObject->FloatAttribute("y");

					Vector2 temp =Vector2(std::ceil(x) * globals::SPRITE_SCALE,
						std::ceil(y) * globals::SPRITE_SCALE);
					//std::cout << "bat at: " << temp.x<<temp.y << std::endl;
					_enemiesList.push_back(new Bat(graphics, temp));

					pObject = pObject->NextSiblingElement("object");
				}
			}
		}
		if (ss.str() == "Slopes") {
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject) {
				while (pObject) {//loop through each object
					float p1X = pObject->FloatAttribute("x");
					float p1Y = pObject->FloatAttribute("y");
					Vector2 p = Vector2(std::ceil(p1X), std::ceil(p1Y));

					XMLElement* pLine = pObject->FirstChildElement("polyline");
					if (pLine) {
						//read points
						std::vector<Vector2> points;
						std::vector<float> singlePoints;//vector with single points
						std::string temp = pLine->Attribute("points");
						std::istringstream split(temp);
						std::string pair;
						while (getline(split, pair, ' ')) {//split by whitespaces
							std::istringstream split2(pair);
							std::string single;
							while (getline(split2, single, ',')) {//split by commas
								//THINK ABOUT IF IT'S NEGATIVE MAYBE IS BETTER TO FLOOR INSTEAD OF CEIL
								//singlePoints.push_back(std::ceil(stof(single)));
								singlePoints.push_back( stof(single) > 0 ?
									std::ceil(stof(single)) :
									std::floor(stof(single))
								);
							}
						}
						for (int i = 0; i < singlePoints.size(); i+=2) {//add the single points to the vector of points
							Vector2 tempVec = Vector2(singlePoints.at(i), singlePoints.at(i + 1));
							points.push_back(tempVec);
						}
						//create the slopes
						for (int i = 0; i < points.size() -1; i++) {
							_collisionSlopes.push_back(
								/*Slope(Vector2(p.x + points.at(i < 2 ? i : i - 1).x * globals::SPRITE_SCALE,
											  p.y + points.at(i < 2 ? i : i - 1).y * globals::SPRITE_SCALE),
									  Vector2(p.x + points.at(i < 2 ? i + 1 : i).x * globals::SPRITE_SCALE,
										      p.y + points.at(i < 2 ? i + 1 : i).y * globals::SPRITE_SCALE)
								)*/
								
								Slope(Vector2((p.x + points.at(i).x) * globals::SPRITE_SCALE,
											  (p.y + points.at(i).y) * globals::SPRITE_SCALE),
									  Vector2((p.x + points.at( i + 1 ).x) * globals::SPRITE_SCALE,
											  (p.y + points.at( i + 1 ).y) * globals::SPRITE_SCALE)
								)
							);
						}
					}//end of p line

					pObject = pObject->NextSiblingElement("object");
				}
			}
		}
		if (ss.str() == "Doors") {//if is a spawn point group
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject) {
				while (pObject) {//loop through each object
					float x, y, width, height;
					//query attribute
					x = pObject->FloatAttribute("x");
					y = pObject->FloatAttribute("y");
					width = pObject->FloatAttribute("width");
					height = pObject->FloatAttribute("height");
					//add to collision vector
					Rectangle rect(//ceil will round up the floats
						std::ceil(x) * globals::SPRITE_SCALE,
						std::ceil(y) * globals::SPRITE_SCALE,
						std::ceil(width) * globals::SPRITE_SCALE,
						std::ceil(height) * globals::SPRITE_SCALE
					);

					XMLElement* pProperties = pObject->FirstChildElement("properties");
					if (pProperties) {
						while (pProperties) {
							XMLElement* pProperty = pProperties->FirstChildElement("property");
							if (pProperty) {
								while (pProperty) {
									std::string value = pProperty->Attribute("value");
									Door door(rect, value);
									_doorList.push_back(door);

									pProperty = pProperty->NextSiblingElement("property");
								}
							}
							pProperties = pProperties->NextSiblingElement("properties");
						}
					}

					pObject = pObject->NextSiblingElement("object");
				}
			}
		}
		pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");//go to next object group
	}
}

//adds a rectangle to the list of possibles collisions for the player
void Level::addCollisionRectangle(tinyxml2::XMLElement * pObject) {
	float x, y, width, height;
	//query attribute
	x = pObject->FloatAttribute("x");
	y = pObject->FloatAttribute("y");
	width = pObject->FloatAttribute("width");
	height = pObject->FloatAttribute("height");
	//add to collision vector
	_collisionRect.push_back(Rectangle(//ceil will round up the floats
		std::ceil(x) * globals::SPRITE_SCALE,
		std::ceil(y) * globals::SPRITE_SCALE,
		std::ceil(width) * globals::SPRITE_SCALE,
		std::ceil(height) * globals::SPRITE_SCALE
	));
}
void Level::addSlopeRectangle(tinyxml2::XMLElement * pObject, std::string value = "") {
	float x, y, width, height;
	//query attribute
	x = pObject->FloatAttribute("x");
	y = pObject->FloatAttribute("y");
	width = pObject->FloatAttribute("width");
	height = pObject->FloatAttribute("height");
	
	//add to collision vector
	_slopesRect.push_back(Rectangle(//ceil will round up the floats
		std::ceil(x) * globals::SPRITE_SCALE,
		std::ceil(y) * globals::SPRITE_SCALE,
		std::ceil(width) * globals::SPRITE_SCALE,
		std::ceil(height) * globals::SPRITE_SCALE,
		value == "left" ? true : false
	));
}

void Level::update(float elapsedTime, Player &player) {
	for (int i = 0; i <  _enemiesList.size(); i++) {
		if (_enemiesList.at(i)->getDie()) {
			_enemiesList.erase(_enemiesList.begin()+ i);
		}
		else {
			_enemiesList.at(i)->update(elapsedTime, player);
		}
	}
}

void Level::draw(Graphics &graphics) {
	for (int i = 0; i < _tileList.size(); i++) {
		_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < _enemiesList.size(); i++) {
		_enemiesList.at(i)->draw(graphics);
	}
}