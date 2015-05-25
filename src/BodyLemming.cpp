#include "BodyLemming.h"

BodyLemming::BodyLemming(Semantic* type) : Body(type)
{
}

Perception* BodyLemming::getPerception()
{
    return m_perception;
}
