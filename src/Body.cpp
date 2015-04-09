#include "Body.h"


Body::Body(Semantic semantic)
{
	this->semantic = semantic;
	this->posX = rand()%800;
	this->posY = rand()%800;
}

std::vector<PhysicalObject*>* Body::GetPerception()
{
	return NULL;
}

void Body::SetIntention(float dx, float dy, float velocity)
{
	this->posX += dx * velocity;
	this->posY += dy * velocity;
}

void Body::SetPosition(float x, float y)
{
	this->posX = x;
	this->posY = y;
}

void Body::GetPosition(float &x, float &y)
{
	x = this->posX;
	y = this->posY;
}

std::vector<float> Body::GetPosition()
{
	std::vector<float> ret;
	ret.push_back(this->posX);
	ret.push_back(this->posY);
	return ret;
}
	
void Body::SetSemantic(Semantic semantic)
{
	this->semantic = semantic;
}

Semantic* Body::GetSemantic()
{
	return &this->semantic;
}

Body::~Body(void)
{
}
