#ifndef MAPGENERATOR_H_
#define MAPGENERATOR_H_

#include "World.h"

class World;

class MapGenerator
{
private:
	World* world;

public:
	MapGenerator(void);

	void setWorld(World* world);
	World* getWorld();

	~MapGenerator(void);
};

#endif
