#include "BodyLemmingMemory.h"

BodyLemmingMemory::BodyLemmingMemory(SEMANTIC type) : BodyLemming(type)
{
}

void BodyLemmingMemory::serialize(pugi::xml_node * node)
{
    BodyLemming::serialize(node);
}

Memory* BodyLemmingMemory::getMemory()
{
    return &this->memory;
}

// Redefining from PhysicalObject to add memory
void BodyLemmingMemory::setPosition(int x, int y)
{
	this->memory.addPosition(x, y);
	BodyLemming::setPosition(x, y);
}
