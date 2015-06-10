#include "Perception.h"

Perception::Perception(int lemmingX, int lemmingY, int exitX, int exitY) : lemmingX(lemmingX), lemmingY(lemmingY), exitX(exitX), exitY(exitY)
{

}

std::vector<PhysicalObject*>* Perception::getPerceivedObjects()
{
	return &this->perceivedObjects;
}

void Perception::display()
{
	std::cout << "Displaying perception" << std::endl;
	int i = 0;
	for (std::vector<PhysicalObject*>::iterator it = this->perceivedObjects.begin(); it != this->perceivedObjects.end(); ++it)
	{
		if ((*it) == NULL)
			std::cout << "Perception[" << i << "] : EMPTY" << std::endl;
		else
			std::cout << "Perception[" << i << "] : " << (*it)->getSemantic() << std::endl;
		
		++i;
	}
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