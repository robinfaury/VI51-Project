#include "PerceptionCross.h"

PerceptionCross::PerceptionCross()
{
}

PerceptionCross::PerceptionCross(std::vector<PhysicalObject*> percievedObjects)
{
	setPercievedObjects(percievedObjects);
}

void PerceptionCross::setPercievedObjects(std::vector<PhysicalObject*> percievedObjects)
{
	this->percievedObjects.clear();

	this->percievedObjects.push_back(percievedObjects.at(1));
	this->percievedObjects.push_back(percievedObjects.at(3));
	this->percievedObjects.push_back(percievedObjects.at(4));
	this->percievedObjects.push_back(percievedObjects.at(6));
}