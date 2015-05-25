#include "PerceptionCircle.h"

PerceptionCircle::PerceptionCircle()
{
}

PerceptionCircle::PerceptionCircle(std::vector<PhysicalObject*> perceivedObjects)
{
	setPerceivedObjects(perceivedObjects);
}

void PerceptionCircle::setPerceivedObjects(std::vector<PhysicalObject*> perceivedObjects)
{
	this->perceivedObjects.clear();
	this->perceivedObjects = perceivedObjects;
}