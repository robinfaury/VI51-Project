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

Cell* Map::getCell(int x, int y)
{
    return getCell(std::pair<int, int>(x, y));
}

Cell* Map::getCell(std::pair<int, int> cell)
{
    return (m_map.find(cell)->second);
}
