#include "PerceptionCircle.h"

PerceptionCircle::PerceptionCircle()
{
}

std::vector<PhysicalObject*>* PerceptionCircle::getPercievedObjects()
{
    return &this->percievedObjects;
}
