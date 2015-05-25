#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include <vector>

#include "Semantic.h"

class PhysicalObject
{
protected:
	Semantic* type;
	int posX;
	int posY;

	PhysicalObject(void) {}

public:
	PhysicalObject(Semantic* type);

	virtual void setPosition(int x, int y) = 0;
	virtual void getPosition(int &x, int &y) = 0;
	virtual std::vector<int> getPosition() = 0;

	virtual void setSemantic(Semantic* type) = 0;
	virtual Semantic* getSemantic() = 0;

	~PhysicalObject(void);
};

#endif
