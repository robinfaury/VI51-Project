#include "PerceptionCircle.h"

PerceptionCircle::PerceptionCircle(int lemmingX, int lemmingY, int exitX, int exitY) : Perception(lemmingX, lemmingY, exitX, exitY)
{
}

void PerceptionCircle::setPerceivedObjects(std::vector<PhysicalObject*> perceivedObjects)
{
	this->perceivedObjects.clear();
	this->perceivedObjects = perceivedObjects;
}