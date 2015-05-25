#include "Body.h"


Body::Body(SEMANTIC type) : PhysicalObject(type)
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

Body::~Body(void)
{

}

void Body::setMap(std::map<std::pair<int, int>, Cell*>* pMap) 
{ 
	this->m_map = pMap; 
}

void Body::destroy()
{

}