#include "BodyLemming.h"

BodyLemming::BodyLemming(SEMANTIC type) : Body(type)
{
}

Perception* BodyLemming::getPerception()
{
    return m_perception;
}

void BodyLemming::serialize(pugi::xml_node * node)
{
	Body::serialize(node);
}