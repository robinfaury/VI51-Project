#include "PerceptionCircle.h"

PerceptionCircle::PerceptionCircle()
{
}

PerceptionCircle::PerceptionCircle(std::vector<PhysicalObject*> percievedObjects)
{
	setPercievedObjects(percievedObjects);
}

void PerceptionCircle::setPercievedObjects(std::vector<PhysicalObject*> percievedObjects)
{
	this->percievedObjects.clear();
	this->percievedObjects = percievedObjects;
}