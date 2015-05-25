#include "Body.h"


Body::Body(Semantic* type) : PhysicalObject(type)
{

}

void Body::setPerception(Perception* newPerception)
{
    delete(m_perception);
    m_perception = newPerception;
}

void Body::setInfluence(ACTIONS influence)
{
	this->m_influence = influence;
}

ACTIONS Body::getInfluence()
{
	return m_influence;
}

void Body::setPosition(int x, int y)
{
	this->posX = x;
	this->posY = y;
}

void Body::getPosition(int &x, int &y)
{
	x = this->posX;
	y = this->posY;
}

std::vector<int> Body::getPosition()
{
	std::vector<int> ret;
	ret.push_back(this->posX);
	ret.push_back(this->posY);
	return ret;
}

void Body::setSemantic(Semantic* type)
{
    delete (this->type);
	this->type = type;
}

Semantic* Body::getSemantic()
{
	return this->type;
}

Body::~Body(void)
{

}
