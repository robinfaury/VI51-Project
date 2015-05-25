#include "Perception.h"

Perception::Perception()
{

}

std::vector<PhysicalObject*>* Perception::getPerceivedObjects()
{
	return &this->perceivedObjects;
}

void Perception::getLemmingPos(int& x, int& y)
{
	x = this->lemmingX;
	y = this->lemmingY;
}

int Perception::getLemmingX()
{
	return this->lemmingX;
}

int Perception::getLemmingY()
{
	return this->lemmingY;
}

void Perception::getExitPos(int& x, int& y)
{
	x = this->exitX;
	y = this->exitY;
}

int Perception::getExitX()
{
	return this->exitX;
}

int Perception::getExitY()
{
	return this->exitY;
}