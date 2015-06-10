#include "PerceptionCross.h"

PerceptionCross::PerceptionCross(int lemmingX, int lemmingY, int exitX, int exitY) : Perception(lemmingX, lemmingY, exitX, exitY)
{
}

void PerceptionCross::setPerceivedObjects(std::vector<PhysicalObject*> percievedObjects)
{
	this->perceivedObjects.clear();

	this->perceivedObjects.push_back(percievedObjects.at(1));
	this->perceivedObjects.push_back(percievedObjects.at(3));
	this->perceivedObjects.push_back(percievedObjects.at(4));
	this->perceivedObjects.push_back(percievedObjects.at(6));
}