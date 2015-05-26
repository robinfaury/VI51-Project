#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <glm.hpp>
#include <typeinfo>

#include "PhysicalObject.h"
#include "BodyLemming.h"
#include "Event.h"
#include "PerceptionCircle.h"
#include "Terrain.h"

class World
{
private:
    Map* m_map; // current map

    std::vector<PhysicalObject*> m_objects;    // All PhysicalObjects (which aren't bodies)
	std::vector<Body*> m_bodies;    // All bodies in the world
	std::vector<ACTIONS> m_influences;    // All influences of the bodies

	bool isDiggable(SEMANTIC semantic);		// For a given semantic, returns true if the lemming can dig it
    void setBodyPerception(Body* body);		// Sets the perception of given body

	std::string currentLevelPath;

public:
	World(void);
	~World(void);

    // Initialise the map (first time init)
	void createMap();

	// Loading/saving level
	//TODO: save level
	void loadLevel(std::string path = "Default");

    // Create a body at position x / y
	Body* createBody(int x, int y);
	PhysicalObject* createObject(int x, int y, SEMANTIC type);
	void removeObject(PhysicalObject* object);	// Completely removes given object from the map and the object container
	void removeObject(int x, int y);	// Completely removes object at given position from the map and the object container

    // Influence stuff
	void collectInfluences();
	void resolveInfluences();	

    // General getters & setters
	std::vector<Body*>* getBodies();
	Map* getMap();

    // Run the world for one tick
	void update();

	// Set the perceptions of all bodies
	void setPerceptions();

	//Reset the current level
	void reset();

};

#endif
