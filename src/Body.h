#ifndef BODY_H_
#define BODY_H_

#include <vector>
#include <stdlib.h>

#include "PhysicalObject.h"

class Body:  public PhysicalObject
{
private:
	Body(void) {}

public:
	Body(Semantic semantic);

	std::vector<PhysicalObject*>* GetPerception();
	void SetIntention(float dx, float dy, float velocity);

	virtual void SetPosition(float x, float y);
	virtual void GetPosition(float &x, float &y);
	virtual std::vector<float> GetPosition();

	virtual void SetSemantic(Semantic semantic);
	virtual Semantic* GetSemantic();

	~Body(void);
};

#endif
