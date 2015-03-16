#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include <vector>

#include "Semantic.h"

class PhysicalObject
{
protected:
	Semantic type;
	float posX;
	float posY;

	PhysicalObject(void) {}

public:
	PhysicalObject(Semantic type);

	virtual void SetPosition(float x, float y) = 0;
	virtual void GetPosition(float &x, float &y) = 0;
	virtual std::vector<float> GetPosition() = 0;
	
	virtual void SetSemantic(Semantic type) = 0;
	virtual Semantic* GetSemantic() = 0;

	~PhysicalObject(void);
};

#endif