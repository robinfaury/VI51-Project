#ifndef CELL_H_
#define CELL_H_

#include "PhysicalObject.h"

class Cell
{
private:
	PhysicalObject* m_worldObject;

public:
	Cell(void);
	Cell(PhysicalObject* worldObject);
	~Cell(void);

	void setWorldObject(PhysicalObject* worldObject);
	PhysicalObject* getWorldObject();
};

#endif
