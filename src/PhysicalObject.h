#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include <vector>

#include "Semantic.h"

class PhysicalObject
{
protected:
	SEMANTIC type;
	int posX;
	int posY;

public:
	PhysicalObject(SEMANTIC type);

	virtual void setPosition(int x, int y);
	virtual void getPosition(int &x, int &y);
	virtual std::vector<int> getPosition();

	virtual void setSemantic(SEMANTIC type);
	virtual SEMANTIC getSemantic();

	virtual void destroy() = 0;

	~PhysicalObject(void);
};

#endif
