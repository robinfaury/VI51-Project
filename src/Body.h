#ifndef BODY_H_
#define BODY_H_

#include <vector>

#include "PhysicalObject.h"

class Body:  public PhysicalObject
{
private:
	Body(void) {}

public:
	Body(Semantic type);

	std::vector<PhysicalObject*>* GetPerception();
	void SetIntention(float dx, float dy, float velocity);

	virtual void SetPosition(float x, float y);
	virtual void GetPosition(float &x, float &y);
	virtual std::vector<float> GetPosition();
	
	virtual void SetSemantic(Semantic type);
	virtual Semantic* GetSemantic();

	~Body(void);
};

#endif