#ifndef CELL_H_
#define CELL_H_



#include "PhysicalObject.h"

/** \class Cell
*   The Cell class represents one cell in the environment grid.
*   A cell contains a pointer to a worldbject. If it's null, it means the cell is empty.
*/
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
