#include "MapGenerator.h"


MapGenerator::MapGenerator(int size) : sizeMap(size)
{
}

void MapGenerator::generateWithAutoSeeds()
{
	generate(1.25*this->sizeMap, 0.5*this->sizeMap);
}

void MapGenerator::generate(int nbSeedDirt, int nbSeedRock)
{
	std::cout << "Generating new map for size : " << this->sizeMap << std::endl;

	for (int i=0; i<this->sizeMap; ++i)
	{
		this->world->createObject(0, i, SEMANTIC::T_BOUND);
		this->world->createObject(i, 0, SEMANTIC::T_BOUND);
		this->world->createObject(this->sizeMap-1, i, SEMANTIC::T_BOUND);
		this->world->createObject(i, this->sizeMap-1, SEMANTIC::T_BOUND);
	}

	this->world->createObject(this->sizeMap-2, this->sizeMap-2, SEMANTIC::T_EXIT);
	createSeed(SEMANTIC::T_DIRT, this->sizeMap-3, this->sizeMap-2);
	createSeed(SEMANTIC::T_DIRT, this->sizeMap-2, this->sizeMap-3);

	for (int i=0; i<nbSeedDirt; ++i)
		createSeed(SEMANTIC::T_DIRT, rand()%(this->sizeMap-2) + 1, rand()%(this->sizeMap-2) + 1);
	for (int i=0; i<nbSeedRock; ++i)
		createSeed(SEMANTIC::T_ROCK, rand()%(this->sizeMap-2) + 1, rand()%(this->sizeMap-2) + 1);

	this->listCell.push_back(this->seedDirt);
	this->listCell.push_back(this->seedRock);

	//NbFreeCell is the number of free cell. So it's sizeMap*sizeMap - Bound - seeds - lemming and exit
	int nbFreeCell = this->sizeMap*this->sizeMap - 4*(this->sizeMap-1) - this->seedDirt.size() - this->seedRock.size() - 1;
	int borne, count;
	SEMANTIC type;
	while(nbFreeCell)
	{
		for (int i=0; i<this->listCell.size(); ++i)
		{
			(i == 0)? type=SEMANTIC::T_DIRT: type=SEMANTIC::T_ROCK;
			borne = static_cast<int>(this->listCell[i].size());
			for (int j=0; j<borne; ++j)
			{
				count = 0;
				(createObject(this->listCell[i][j].first-1, this->listCell[i][j].second, type) == 1)? --nbFreeCell: ++count;
				(createObject(this->listCell[i][j].first+1, this->listCell[i][j].second, type) == 1)? --nbFreeCell: ++count;
				(createObject(this->listCell[i][j].first, this->listCell[i][j].second-1, type) == 1)? --nbFreeCell: ++count;
				(createObject(this->listCell[i][j].first, this->listCell[i][j].second+1, type) == 1)? --nbFreeCell: ++count;

				if (count == 4)
				{
					this->listCell[i].erase(this->listCell[i].begin()+j);
					--borne;
					--j;
				}
			}
		}
	}

	for (int i=0; i<10; i)
	{
		int x = rand()%(this->sizeMap-2)-1;
		int y = rand()%(this->sizeMap-2)-1;
		if (this->world->getObject(x, y)->getSemantic() == SEMANTIC::T_DIRT)
		{
			this->world->removeObject(x, y);
			this->world->createBody(x, y);
			++i;
		}
	}

	std::cout << "Generated new map" << std::endl;
}

bool MapGenerator::createObject(int x, int y, SEMANTIC type)
{
	int id;
	(type == SEMANTIC::T_DIRT)? id=0: id=1;
	if (this->world->createObject(x, y, type) != NULL)
	{
		std::pair<int, int> positionNewBlock;
		positionNewBlock.first = x;
		positionNewBlock.second = y;
		this->listCell[id].push_back(positionNewBlock);
		return true;
	}
	return false;
}

void MapGenerator::createSeed(SEMANTIC type, int x, int y)
{
	std::pair<int, int> seed;
	seed.first = x;
	seed.second = y;
	if (this->world->createObject(seed.first, seed.second, type) != NULL)
	{
		if (type == SEMANTIC::T_DIRT)
			this->seedDirt.push_back(seed);
		else
			this->seedRock.push_back(seed);
	}
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
