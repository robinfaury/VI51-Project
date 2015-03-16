#include "World.h"


World::World(void)
{
}

Body* World::CreateBody()
{
	this->listOfPhysicalObjects.push_back(new Body(BasicAgent()));
	Body* body = static_cast<Body*>(this->listOfPhysicalObjects.at(this->listOfPhysicalObjects.size()-1));
	this->listOfBodys.push_back(body);
	return body;
}

void World::CreateWall()
{

}

std::vector<PhysicalObject*>* World::GetListOfPhysicalObjects() 
{
	return &this->listOfPhysicalObjects;
}

std::vector<Body*>* World::GetListOfBodys() 
{
	return &this->listOfBodys;
}

World::~World(void)
{
}
