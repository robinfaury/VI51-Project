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
#include "pugi/pugixml.hpp"

#define MAP_DEBUG 0

using namespace std;

class Map
{
public:
	Map(string path = "Map/Default");
	~Map();
	void clearMap();

	std::map<std::pair<int, int>, Cell*>* getMap();

    /*
	void unserialize(pugi::xml_node mapNode);
	void serialize(pugi::xml_node* mapNode);
	*/

	//map management

	void createCell(int x, int y, PhysicalObject* object);  // Create a cell at given position

	bool isCellCreated(int x, int y);   // Returns true if cell x/y has been created already
	bool isCellCreated(std::pair<int, int> cell);   // Overload
	bool isCellEmpty(int x, int y);	// If cell is created and empty, returns true. Else, false.

	Cell* getCell(int x, int y);
	Cell* getCell(std::pair<int, int> cell);

	// Object management
	bool addWorldObject(int x, int y, PhysicalObject* object);	// Adds object to the map. If cell doesn't exist, create it. If cell exists but is already occupied, return false and do nothing
	bool removeWorldObject(PhysicalObject* object);	// Returns true if given object has been removed from the map. Else, returns false.
	bool moveWorldObject(int oldX, int oldY, int newX, int newY);	// Moves object to given cell (if it's empty), and returns true. If move is impossible (cell doesn't exist, or is occupied), return false.

	std::map<std::pair<int, int>, Cell*>::iterator findCell(Cell* cell);	// Returns an iterator to given cell. Returns an iterator to end if it can't be found.
	std::map<std::pair<int, int>, Cell*>::iterator findCell(int x, int y);	// Returns an iterator to the cell with given coordinates. Returns an iterator to end if it can't be found.
	std::map<std::pair<int, int>, Cell*>::iterator findCell(PhysicalObject* object);	// Returns an iterator to the cell containing the given object. Returns an iterator to end if it can't be found.
protected:
    // The world, represented as a grid of cells
	std::map<std::pair<int, int>, Cell*> m_map;

};

#endif
