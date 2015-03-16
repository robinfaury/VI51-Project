#ifndef WORLD_H_
#define WORLD_H_

#include <vector>

#include "Body.h"
#include "BasicAgent.h"

class World
{
private:
	std::vector<PhysicalObject*> listOfPhysicalObjects;
	std::vector<Body*> listOfBodys;

public:
	World(void);

	Body* CreateBody();
	void CreateWall();

	std::vector<PhysicalObject*>* GetListOfPhysicalObjects();
	std::vector<Body*>* GetListOfBodys();

	~World(void);
};

#endif