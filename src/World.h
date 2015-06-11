#ifndef WORLD_H_
#define WORLD_H_

/** \class World
*   The World class represents the environment.
*   The world contains the map (which is a grid of cell), and containers to store objects, bodies, etc.
*/

#define DEFAULT_SIZE 10
#define WORLD_DEBUG 0

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <glm.hpp>
#include <typeinfo>

#include "PhysicalObject.h"
#include "BodyLemming.h"
#include "PerceptionCircle.h"
#include "PerceptionCross.h"
#include "Terrain.h"
#include "MapGenerator.h"

#define extensionPath ".xml"
#define resPath "res/"
#define mapPath "maps/"

class MapGenerator;

class World
{
private:
    Map* m_map; // current map
	MapGenerator* mapGenerator;

	int size;

    std::vector<PhysicalObject*> m_objects;    // All PhysicalObjects (which aren't bodies)
	std::vector<Body*> m_bodies;    // All bodies in the world
	std::vector<ACTIONS> m_influences;    // All influences of the bodies

	bool isDiggable(SEMANTIC semantic);		// For a given semantic, returns true if the lemming can dig it
    void setBodyPerception(Body* body);		// Sets the perception of given body

	std::string* currentMap;
	int exitX;
	int exitY;

	PhysicalObject* deserializeObject(pugi::xml_node* objectNode);

public:
	World(std::string* levelPath);
	~World(void);

	// Loading/saving level
	//TODO: save level
	/**
	*   This function saves the level in the given filepath, under xml format
	*/
	void saveLevel(std::string path);

	bool loadLevel(std::string path = "Generate");
	void resetMap();	// Resets map with current level path
	void generateLevel();
	void findExit(int& exitX, int& exitY);	// finds the exit and sets its position to the two given integers

	int getSize();
	void setSize(int size);	// Sets the size of the map (in number of cells square)

    /**
    *   Creates a body at given positon, stores it in the map, and returns a pointer to it
    */
	Body* createBody(int x, int y);

	/**
	*   Creates an object at given position (with given semantic), stores it in the map, and returns a pointer to it
	*/
	PhysicalObject* createObject(int x, int y, SEMANTIC type);

	/**
	*   Completely removes an object of the simulation
	*/
	void removeObject(PhysicalObject* object);	// Completely removes given object from the map and the object container

	/**
	*   If there's an object at given position, completely removes it from the simulation
	*/
	void removeObject(int x, int y);	// Completely removes object at given position from the map and the object container

    // Influence stuff
    /**
    *   Collect influences from all bodies
    */
	void collectInfluences();

	/**
	*   Resolve influences from all bodies
	*/
	void resolveInfluences();

    // General getters & setters
    /**
    *   Returns the list of bodies
    */
	std::vector<Body*>* getBodies();
	Map* getMap();

    /**
    *   Udpdates the world. This function is called everyframe.
    */
	void update();

	/**
	*   Called everyframe. This function sets the new perception of the bodies for the current frame.
	*/
	void setPerceptions();

	//TODO: Milan do this
	/**
	*   Called by the LearningMethods. This function returns what would a body percieve if it was standing at this position
	*/
	Perception* getPerceptionFromTile(int tileX, int tileY);	// Gets perception from given tile.

	// Returns true if given position is valid (either dirt, empty, exit, or lemming)
	bool checkValidPosition(int tileX, int tileY);

	// Forces the lemming on given position
	bool forceLemmingPosition(int tileX, int tileY);


    /**
*   Called by the LearningMethods. This function returns all possible lemming perceptions (ignoring above objects)
*
*   Perceptions are stored like this :
*   ?   ?   ?
*   [0] X   [1]
*   [2] [3] [4]
*
*   where ? is unknown (and not contained in the vector), and X is the lemming
*/
    std::vector<std::vector<PhysicalObject*>> getAllPossiblePerceptions();

};

#endif
