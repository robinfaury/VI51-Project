#include "BodyLemming.h"

BodyLemming::BodyLemming(SEMANTIC type) : Body(type)
{
}

Perception* BodyLemming::getPerception()
{
    return m_perception;
}
