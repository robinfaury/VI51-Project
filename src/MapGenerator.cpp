#include "MapGenerator.h"


MapGenerator::MapGenerator()
{
	
}

void MapGenerator::setWorld(World* world)
{
	this->world = world;
}

World* MapGenerator::getWorld()
{
	return this->world;
}

MapGenerator::~MapGenerator()
{
	
}
