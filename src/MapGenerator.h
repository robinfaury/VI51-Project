#ifndef MAPGENERATOR_H_
#define MAPGENERATOR_H_

#include "World.h"

class World;

class MapGenerator
{
private:
	World* world;
	int sizeMap;

	std::vector<std::pair<int, int> > seedDirt;
	std::vector<std::pair<int, int> > seedRock;
	std::vector<std::vector<std::pair<int, int> > > listCell;

	void createSeed(SEMANTIC type);
	bool createObject(int x, int y, SEMANTIC type);

public:
	MapGenerator(void);

	void generate(int nbSeedDirt, int nbSeedRock);
	void generateWithAutoSeeds();

	void setWorld(World* world);
	World* getWorld();

	~MapGenerator(void);
};

#endif
