#include "Cell.h"


Cell::Cell(void) : m_worldObject(0)
{
}

Cell::Cell(PhysicalObject* worldObject)
{
    m_worldObject = worldObject;
}

Cell::~Cell(void)
{
	// Objects will be deleted by the world.
}

void Cell::setWorldObject(PhysicalObject* worldObject)
{
    m_worldObject = worldObject;
}

PhysicalObject* Cell::getWorldObject()
{
    return m_worldObject;
}
