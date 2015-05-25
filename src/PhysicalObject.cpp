#include "PhysicalObject.h"


PhysicalObject::PhysicalObject(SEMANTIC type)
{
	this->type = type;
}


PhysicalObject::~PhysicalObject(void)
{
}


void PhysicalObject::setPosition(int x, int y)
{
	this->posX = x;
	this->posY = y;
}

void PhysicalObject::getPosition(int &x, int &y)
{
	x = this->posX;
	y = this->posY;
}

std::vector<int> PhysicalObject::getPosition()
{
	std::vector<int> ret;
	ret.push_back(this->posX);
	ret.push_back(this->posY);
	return ret;
}

void PhysicalObject::setSemantic(SEMANTIC type)
{
	this->type = type;
}

SEMANTIC PhysicalObject::getSemantic()
{
	return this->type;
}