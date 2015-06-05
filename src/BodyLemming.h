#ifndef BODYLEMMING_H_
#define BODYLEMMING_H_

#include "Body.h"

class BodyLemming : public Body
{
private:


public :
	BodyLemming(SEMANTIC type);

    virtual Perception* getPerception();

	virtual void serialize(pugi::xml_node * node);
};

#endif
