#include "Map.h"


Map::Map(string path)
{
    //TODO: redo
}


Map::~Map()
{
}

std::map<std::pair<int, int>, Cell*>* Map::getMap()
{
    return &m_map;
}

/*
void Map::unserializeMap(pugi::xml_node mapNode)
{
//TODO: clean
	m_mapHeight = 0;
	m_mapWidth = 0;

	//string completePath = "Assets/";
	//completePath.append(path);

	// Loading map information
	//TODO: map name, name handling?
	pugi::xml_node tempNode = mapNode.child("MapInfo");
	pugi::xml_attribute tempAttribute = tempNode.attribute("width");
	m_mapWidth = tempAttribute.as_int();

	tempAttribute = tempNode.attribute("height");
	m_mapHeight = tempAttribute.as_int();
	addTiles(getTileNumber());

	cout << "map width :  " << m_mapWidth << endl;
	cout << "map height :  " << m_mapHeight << endl;

	// Loading tiles
	tempNode = mapNode.child("Tiles");
	pugi::xml_node tempTile = tempNode.first_child();
	Tile* tile;

	if (!tempTile)
	{
		cout << "OH CEYLAMERDEBLFBLFBLFBLF" << endl;
	}

	while (tempTile)
	{
		int x = tempTile.attribute("x").as_int();
		int y = tempTile.attribute("y").as_int();
		TILE_TYPE_PHYSICAL physicalType;
		TILE_TYPE_GRAPHICAL graphicalType;
		tile = getTile(x, y);
		if (tile != NULL)
		{
			physicalType = static_cast<TILE_TYPE_PHYSICAL>(tempTile.attribute("physicalType").as_int());
			graphicalType = static_cast<TILE_TYPE_GRAPHICAL>(tempTile.attribute("graphicalType").as_int());

			if (physicalType == TILE_TYPE_PHYSICAL::T_PLAYERSTART)
			{
				m_playerStart.x = (float)x;
				m_playerStart.y = (float)y;
				tile->setType_physical(TILE_TYPE_PHYSICAL::T_EMPTY);
				tile->setType_graphical(graphicalType);
			}
			else
			{
				tile->setType_physical(physicalType);
				tile->setType_graphical(graphicalType);
			}

			//cout << "Tile " << x << "," << y << " : " << type << endl;
		}
		else
		{
			cout << "Map : deserializeMap : ERROR couldn't find corresponding tile!" << endl;
		}

		tempTile = tempTile.next_sibling();
	}

	cout << "Map loaded" << endl;
}

void Map::serializeMap(pugi::xml_node* mapNode)
{
	cout << "Map : serializeMap : begin" << endl;

	cout << "Map : serializeMap : mapInfo" << endl;
	// Saving map info
	pugi::xml_node mapInfo = mapNode->append_child("MapInfo");
	pugi::xml_attribute mapWidth = mapInfo.append_attribute("width");
	mapWidth.set_value(m_mapWidth);

	pugi::xml_attribute mapHeight = mapInfo.append_attribute("height");
	mapHeight.set_value(m_mapHeight);

	// saving tiles
	cout << "Map : serializeMap : Tiles" << endl;
	pugi::xml_node tiles = mapNode->append_child("Tiles");
	pugi::xml_node tempTile;
	pugi::xml_attribute tempTileType;

	for (int i = 0; i < m_mapWidth; ++i)
	{
		for (int j = 0; j < getMapHeight(); ++j)
		{
			tempTile = tiles.append_child("Node");
			tempTile.append_attribute("x").set_value(i);
			tempTile.append_attribute("y").set_value(j);

			//TODO: put that on one line
			tempTileType = tempTile.append_attribute("physicalType");
			tempTileType.set_value(static_cast<int>(getTile(i, j)->getType_physical()));

			//TODO: put that on one line
			tempTileType = tempTile.append_attribute("graphicalType");
			tempTileType.set_value(static_cast<int>(getTile(i, j)->getType_graphical()));
		}
	}
	mapNode->set_name("Map");

	cout << "Map : serializeMap : Done" << endl;
}*/

void Map::clear()	// resets the current map to empty all the way
{
	for (std::map<std::pair<int, int>, Cell*>::iterator it = this->m_map.begin(); it != this->m_map.end(); ++it)
	{
		delete (*it).second;
	}
	m_map.clear();
}

void Map::createCell(int x, int y, PhysicalObject* object)  // Create a cell at given position
{
    if (!isCellCreated(x, y))
    {
        Cell* newCell = new Cell(object);
        std::pair<int, int> newPair(x,y);

        m_map.insert(std::pair<std::pair<int,int>,Cell*>(newPair, newCell));
    }
}

bool Map::isCellCreated(int x, int y)   // Checks if cell is created
{
    return isCellCreated(std::pair<int, int>(x, y));
}

bool Map::isCellCreated(std::pair<int, int> cell)
{
    return (m_map.find(cell) != m_map.end());
}

bool Map::isCellEmpty(int x, int y)	// If cell is created and empty, returns true. Else, false
{
	if (isCellCreated(x, y) && getCell(x, y)->getWorldObject() == NULL)
		return true;
	return false;
}

Cell* Map::getCell(int x, int y)
{
    return getCell(std::pair<int, int>(x, y));
}

Cell* Map::getCell(std::pair<int, int> cell)
{
	std::map<std::pair<int, int>, Cell*>::iterator it = this->m_map.find(cell);
	if (it == this->m_map.end())
		return NULL;
	return (it->second);
}


bool Map::addWorldObject(int x, int y, PhysicalObject* object)
{
	Cell* cell = getCell(x,y);
	if (cell != NULL)
	{
		if (cell->getWorldObject() == NULL)
		{
			//Cell empty, can add
			cell->setWorldObject(object);
			return true;
		}
		else
		{
			// Cell already occupied
			cout << "ERROR : Map::addWorldObject : can't add object to cell " << x << ", " << y << " : cell already occupied" << endl;
			return false;
		}
	}
	else
	{
		// Create cell, and add object
		createCell(x, y, object);
		return true;
	}
}

// Returns true if given object has been removed from the map. Else, returns false.
bool Map::removeWorldObject(PhysicalObject* object)
{
	std::map<std::pair<int, int>, Cell*>::iterator it = findCell(object);
	if (it == this->m_map.end())
	{
		std::cout << "Map::removeWorldObject : couldn't find object in the map." << endl;
		return false;
	}
	else
	{
		it->second->setWorldObject(NULL);
		if (MAP_DEBUG)
			std::cout << "Map::removeWorldObject : removed object at " << it->first.first << "," << it->first.second << endl;
		return true;
	}
}

// Moves object to given cell (if it's empty), and returns true. If move is impossible (cell doesn't exist, or is occupied), return false.
bool Map::moveWorldObject(int oldX, int oldY, int newX, int newY)
{
	//std::cout << "Map::moveWorldObject : moving from " << oldX << "," << oldY << " to " << newX << "," << newY << endl;
	Cell* oldCell = getCell(oldX, oldY);
	Cell* newCell = getCell(newX, newY);

	if (oldCell == NULL || newCell == NULL)
	{
		std::cout << "ERROR : Map::moveWorldObject : invalid parameters" << endl;
		return false;
	}
	else if (oldCell->getWorldObject() == NULL)
	{
		std::cout << "ERROR : Map::moveWorldObject : oldCell is empty" << endl;
		return false;
	}
	else if (newCell->getWorldObject() != NULL)
	{
		std::cout << "ERROR : Map::moveWorldObject : newCell isn't empty" << endl;
		return false;
	}
	else
	{
		if (MAP_DEBUG)
			std::cout << "Map::moveWorldObject : moved object from " << oldX << "," << oldY << " to " << newX << "," << newY << std::endl;
		newCell->setWorldObject(oldCell->getWorldObject());
		oldCell->setWorldObject(NULL);
		return true;
	}
}


std::map<std::pair<int, int>, Cell*>::iterator Map::findCell(Cell* cell)
{
	for (std::map<std::pair<int, int>, Cell*>::iterator it = this->m_map.begin(); it != this->m_map.end(); ++it)
	{
		if (it->second == cell)
		{
			return it;
		}
	}
	return this->m_map.end();
}

std::map<std::pair<int, int>, Cell*>::iterator Map::findCell(int x, int y)
{
	for (std::map<std::pair<int, int>, Cell*>::iterator it = this->m_map.begin(); it != this->m_map.end(); ++it)
	{
		if (it->first.first == x && it->first.second == y)
		{
			return it;
		}
	}
	return this->m_map.end();
}

// Returns an iterator to the cell containing the given object. Returns an iterator to end if it can't be found.
std::map<std::pair<int, int>, Cell*>::iterator Map::findCell(PhysicalObject* object)
{
	for (std::map<std::pair<int, int>, Cell*>::iterator it = this->m_map.begin(); it != this->m_map.end(); ++it)
	{
		if (it->second->getWorldObject() == object)
		{
			return it;
		}
	}
	return this->m_map.end();
}

