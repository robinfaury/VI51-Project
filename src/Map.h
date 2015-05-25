#ifndef MAP_H_
#define MAP_H_
/*
The Map class represent the in game Map.
A Map is composed of Tiles. A map has a width and a height, and a vector of Tiles.

The Map can serialize and deserialize itself using pugixml nodes.
*/

//#include <pugixml.hpp>

#include <string>
#include <map>
#include <iostream>
#include "Cell.h"

using namespace std;

class Map
{
public:
	Map(string path = "Map/Default");
	~Map();

	std::map<std::pair<int, int>, Cell*>* getMap();

    /*
	void unserialize(pugi::xml_node mapNode);
	void serialize(pugi::xml_node* mapNode);
	*/

	//map management
	void clear();	// Clears the current map, resets to void

	void createCell(int x, int y, PhysicalObject* object);  // Create a cell at given position

	bool isCellCreated(int x, int y);   // Returns true if cell x/y has been created already
	bool isCellCreated(std::pair<int, int> cell);   // Overload

	Cell* getCell(int x, int y);
	Cell* getCell(std::pair<int, int> cell);



protected:
    // The world, represented as a grid of cells
	std::map<std::pair<int, int>, Cell*> m_map;
};

#endif
