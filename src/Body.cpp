#include "Body.h"


Body::Body(SEMANTIC type) : PhysicalObject(type)
{
	m_perception = NULL;
}

Body::~Body()
{
	destroy();
}

void Body::setPerception(Perception* newPerception)
{
	if (m_perception != NULL)
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


void Body::setMap(std::map<std::pair<int, int>, Cell*>* pMap) 
{ 
	this->m_map = pMap; 
}

void Body::destroy()
{
	delete (this->m_perception);
	this->m_perception = NULL;
}

void Body::serialize(pugi::xml_node * node)
{
	PhysicalObject::serialize(node);
}