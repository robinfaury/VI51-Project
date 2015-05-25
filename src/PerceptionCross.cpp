#include "PerceptionCross.h"

PerceptionCross::PerceptionCross()
{
}

PerceptionCross::PerceptionCross(std::vector<PhysicalObject*> perceivedObjects)
{
	setPerceivedObjects(perceivedObjects);
}

void PerceptionCross::setPerceivedObjects(std::vector<PhysicalObject*> percievedObjects)
{
	this->perceivedObjects.clear();

	this->perceivedObjects.push_back(percievedObjects.at(1));
	this->perceivedObjects.push_back(percievedObjects.at(3));
	this->perceivedObjects.push_back(percievedObjects.at(4));
	this->perceivedObjects.push_back(percievedObjects.at(6));
}